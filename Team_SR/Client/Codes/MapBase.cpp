#include "stdafx.h"
#include "MapBase.h"

const DWORD CMapBase::Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;;


CMapBase::CMapBase(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{}

HRESULT CMapBase::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapBase::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMapBase::UpdateGameObject(float fDeltaTime)
{
	Super::UpdateGameObject(fDeltaTime);
	m_pTransformCom->UpdateTransform();
	return _uint();
}

_uint CMapBase::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CMapBase::RenderGameObject()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	for (auto& RefInfo : *_InfosPtr)
	{
		m_pDevice->SetTexture(0, RefInfo.Texture);
		auto _Mtrl = RefInfo.MaterialInfo.ConvertMtrl();
		m_pDevice->SetMaterial(&_Mtrl);
		m_pDevice->SetStreamSource(0, RefInfo.VtxBuf, 0,
			sizeof(Vertex));
		m_pDevice->SetFVF(Vertex::FVF);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0,
			RefInfo.TriangleCount);
	}

	return S_OK;
}

void CMapBase::LoadMap(std::wstring FilePath)
{
	_InfosPtr = std::make_shared<std::vector<Info>>();

	const std::wstring _MtlFileName = FilePath + L"MAP.mtl";
	std::wfstream _MtlStream(_MtlFileName);

	if (!_MtlStream.is_open())
	{
		::MessageBox(nullptr, __FUNCTIONW__,
			L"File Open Fail", 0);
	}

	std::map<wstring, MtrlInfo> _MtrlInfo;
	std::wstring Line;
	std::wstringstream wss;
	std::wstring ToKen;

	while (_MtlStream)
	{
		Line.clear();
		ToKen.clear();
		// 한줄 리딩
		std::getline(_MtlStream, Line);
		wss.str(Line);
		wss.clear();
		wss >> ToKen;

		// 머테리얼 파싱 시작
		if (ToKen == L"newmtl")
		{
			MtrlInfo _Info;
			wss >> _Info.MtrlName;
			std::vector<std::wstring> _FileLines;

			while (true)
			{
				std::getline(_MtlStream, Line);
				_FileLines.push_back(Line);
				if (Line.empty())
					break;
			}

			std::reverse(std::begin(_FileLines), std::end(_FileLines));

			while (!_FileLines.empty())
			{
				// 한줄씩 읽으며 파싱
				ToKen.clear();
				std::wstringstream CurLine;
				CurLine.str(std::move(_FileLines.back()));
				_FileLines.pop_back();

				CurLine >> ToKen;

				if (ToKen == L"Kd")
				{
					CurLine >> _Info.Diffuse.r;
					CurLine >> _Info.Diffuse.g;
					CurLine >> _Info.Diffuse.b;
				}
				else if (ToKen == L"map_Kd")
				{
					CurLine >> _Info.TextureName;
				}
				else if (ToKen == L"Ks")
				{
					CurLine >> _Info.Specular.r;
					CurLine >> _Info.Specular.g;
					CurLine >> _Info.Specular.b;
				}
				else if (ToKen == L"Ns")
				{
					CurLine >> _Info.Shine;
				}
				else if (ToKen == L"illum")
				{
					CurLine >> (_uint&)_Info.Illu;
				}

				if (ToKen.empty())
				{
					_MtrlInfo.insert
					({
						_Info.MtrlName  , _Info
						});
				}
			}
		}
	}

	std::wstring _ObjFileName = FilePath + L"MAP.obj";

	const std::wstring V = L"v";
	const std::wstring Vt = L"vt";
	const std::wstring VN = L"vn";
	const std::wstring F = L"f";
	const std::wstring Mtl = L"usemtl";
	const wchar_t FDelim = L'\/';
	using _VtxType = Vertex;
	size_t VtxElementCount = 3;

	std::wfstream _ObjStream(_ObjFileName);

	if (!_ObjStream.is_open())
		::MessageBox(nullptr, __FUNCTIONW__,
			L"File Open Fail", 0);

	std::vector<_VtxType> _Vtxs;
	std::vector<_vector> _Locations;
	std::vector<_vector> _Normals;
	std::vector<D3DXVECTOR2> _TextureCoords;

	wss.str(L"");
	wss.clear();
	Line.clear();

	std::wstring CurMtrlInfoName;

	while (_ObjStream)
	{
		// Line Read.
		std::getline(_ObjStream, Line);
		wss.str(Line);
		wss.clear();
		ToKen.clear();
		wss >> ToKen;

		if (ToKen == V)
		{
			_vector VertexLocation{ 0,0,0 };
			wss >> VertexLocation.x;
			wss >> VertexLocation.y;
			wss >> VertexLocation.z;
			_Locations.push_back(std::move(VertexLocation));
		}
		else if (ToKen == Vt)
		{
			D3DXVECTOR2 VertexTexCoord{ 0,0 };
			wss >> VertexTexCoord.x;
			wss >> VertexTexCoord.y;
			_TextureCoords.push_back(std::move(VertexTexCoord));
		}
		else if (ToKen == VN)
		{
			_vector VertexNormal{ 0,0,0 };
			wss >> VertexNormal.x;
			wss >> VertexNormal.y;
			wss >> VertexNormal.z;
			_Normals.push_back(std::move(VertexNormal));
		}
		else if (ToKen == Mtl)
		{
			std::wstring MtrlName;
			wss >> MtrlName;
			auto iter = _MtrlInfo.find(MtrlName);
			if (iter != std::end(_MtrlInfo))
			{
				CurMtrlInfoName = iter->first;
			}
		}
		else if (ToKen == F)
		{
			std::wstring ReplaceLine;
			std::getline(wss, ReplaceLine);
			std::replace_copy(std::begin(ReplaceLine), std::end(ReplaceLine),
				std::begin(ReplaceLine), FDelim, L' ');
			wss.str(ReplaceLine);
			wss.clear();
			for (size_t i = 0; i < 3; ++i)
			{
				_int LocationIdx, NormalIdx, TexCoordIdx;
				wss >> LocationIdx;
				wss >> TexCoordIdx;
				wss >> NormalIdx;

				_Vtxs.emplace_back
				(
					_VtxType
					{
						_Locations[LocationIdx - 1],
						_Normals[NormalIdx - 1],
						_TextureCoords[TexCoordIdx - 1]
					}
				);
			}
			ReplaceLine.clear();
		}

		if (ToKen.empty() && !_Vtxs.empty())
		{
			Info _Info;

			if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(Vertex) *
				_Vtxs.size(),
				0, Vertex::FVF, D3DPOOL_MANAGED,
				&_Info.VtxBuf, 0)))
			{
				MessageBox(nullptr, __FUNCTIONW__, nullptr, 0);
				
			}

			Vertex* _VtxBuffer;
			_Info.VtxBuf->Lock(0, 0, (void**)&_VtxBuffer, 0);

			memcpy(_VtxBuffer, _Vtxs.data(), sizeof(Vertex) * _Vtxs.size());
			_Info.TriangleCount = _Vtxs.size() / 3;
			_Vtxs.clear();
			_Info.VtxBuf->Unlock();

			auto iter = _MtrlInfo.find(CurMtrlInfoName);

			if (iter == std::end(_MtrlInfo))
			{
				MessageBox(nullptr, L"	if (iter == std::end ( _MtrlInfo)  ) ", nullptr, 0);
				
			}

			_Info.MaterialInfo = iter->second;

			std::wstring TexName = FilePath;
			TexName += _Info.MaterialInfo.TextureName;


			if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
				TexName.c_str(), &_Info.Texture)))
			{
				
			}

			_InfosPtr->push_back(_Info);
		}

		wss.str(std::wstring{});
		Line.clear();
		wss.clear();
	}

}

//CMapBase* CMapBase::Create(LPDIRECT3DDEVICE9 pDevice)
//{
//	if (nullptr == pDevice)
//		return nullptr;
//
//	CMapBase* pInstance = new CMapBase(pDevice);
//	if (FAILED(pInstance->ReadyGameObjectPrototype()))
//	{
//		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
//		SafeRelease(pInstance);
//	}
//
//	return pInstance;
//}
//
//CGameObject* CMapBase::Clone(void * pArg)
//{
//	CMapBase* pClone = new CMapBase(*this); /* 복사생성자 */
//	SafeAddRef(m_pDevice);
//	if (FAILED(pClone->ReadyGameObject(pArg)))
//	{
//		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
//		SafeRelease(pClone);
//	}
//
//	return pClone;
//}

void CMapBase::Free()
{
	Super::Free();
}
