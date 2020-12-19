#include "stdafx.h"
#include "MapBase.h"
#include "CollisionComponent.h"
#include "DXWrapper.h"
#include "ImGuiHelper.h"
#include "Stage.h"
#include "MainCamera.h"
#include "Player.h"
#include "Vertexs.h"



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

	if (ImGuiHelper::bEditOn)
	{
	
	}
	
	return _uint();
}

_uint CMapBase::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Priority, this)))
		return 0;

	return _uint();
}

HRESULT CMapBase::RenderGameObject()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CubeMapRender();

	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");
	
	{
		_Effect.SetVSConstantData(m_pDevice, "World", MapWorld);
	}

	m_pDevice->SetTexture(_Effect.GetTexIdx("EnvironmentSampler"),
		_CubeTexture.get());
	/*m_pDevice->SetTextureStageState(_Effect.GetTexIdx("EnvironmentSampler"), D3DTSS_TEXCOORDINDEX,
		D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 1);*/

	for (auto& RefInfo : *_WallSubSetInfo)
	{
		m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler") , 
			RefInfo.Diffuse);
		m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"), 
			RefInfo.Specular);
		m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),	
			RefInfo.Normal);

		_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind",
			true);
		_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind",
			true);
		
		_Effect.SetPSConstantData(m_pDevice, "Shine", RefInfo.MaterialInfo.Shine);
		m_pDevice->SetStreamSource(0, RefInfo.VtxBuf, 0,sizeof(Vertex::Texture));
		m_pDevice->SetVertexDeclaration(RefInfo.Decl);
		m_pDevice->SetVertexShader(_Effect.VsShader);
		m_pDevice->SetPixelShader(_Effect.PsShader);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, RefInfo.TriangleCount);
	}

	for (auto& RefInfo : *_FloorSubSetInfo)
	{
		m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),
			RefInfo.Diffuse);
		m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),
			RefInfo.Specular);
		m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),
			RefInfo.Normal);
		
		_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind",
			true);
		_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind",
			true);

		{
			m_pDevice->SetSamplerState(_Effect.GetTexIdx("DiffuseSampler"),
				D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			m_pDevice->SetSamplerState(_Effect.GetTexIdx("DiffuseSampler"),
				D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

			m_pDevice->SetSamplerState(_Effect.GetTexIdx("SpecularSampler"),
				D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			m_pDevice->SetSamplerState(_Effect.GetTexIdx("SpecularSampler"),
				D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

			m_pDevice->SetSamplerState(_Effect.GetTexIdx("NormalSampler"),
				D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			m_pDevice->SetSamplerState(_Effect.GetTexIdx("NormalSampler"),
				D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}

		_Effect.SetPSConstantData(m_pDevice, "Shine", RefInfo.MaterialInfo.Shine);
		m_pDevice->SetStreamSource(0, RefInfo.VtxBuf, 0, sizeof(Vertex::Texture));
		m_pDevice->SetVertexDeclaration(RefInfo.Decl);
		m_pDevice->SetVertexShader(_Effect.VsShader);
		m_pDevice->SetPixelShader(_Effect.PsShader);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, RefInfo.TriangleCount);
	};

	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);

	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	return S_OK;
}
void CMapBase::LoadMap(std::wstring FilePath,
	const mat& MapWorld)
{
	this->MapWorld = MapWorld;

	_WallSubSetInfo = std::shared_ptr<std::vector<SubSetInfo>>(new std::vector<SubSetInfo>, []
		(auto& ptr ) 
		{
			for (auto& CurElement : *ptr)
			{
				CurElement.Release();
			};
			delete ptr;
		});
	std::vector<PlaneInfo> _PolygonPlanes;
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
					_Info.Diffuse.a = 1.f;
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
					_Info.Specular.a = 1.f;
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
	size_t VtxElementCount = 3;

	std::wfstream _ObjStream(_ObjFileName);

	if (!_ObjStream.is_open())
		::MessageBox(nullptr, __FUNCTIONW__,
			L"File Open Fail", 0);

	std::vector<typename Vertex::Texture> _Vertexs;
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

			std::array<typename Vertex::Texture,3ul> _FaceVertexs;

			for (size_t i = 0; i < 3; ++i)
			{
				_int LocationIdx, NormalIdx, TexCoordIdx;
				wss >> LocationIdx;
				wss >> TexCoordIdx;
				wss >> NormalIdx;
				typename Vertex::Texture _Vtx;
				_Vtx.Location = _Locations[LocationIdx - 1];
				_Vtx.Normal = _Normals[NormalIdx - 1];
				_Vtx.TexCoord = _TextureCoords[TexCoordIdx - 1];

				_FaceVertexs[i] = (std::move(_Vtx));
			};

			const auto Tangent_BiNormal = Mesh::CalculateTangentBinormal(
											TempVertexType{_FaceVertexs[0].Location,_FaceVertexs[0].TexCoord },
											TempVertexType{ _FaceVertexs[1].Location,_FaceVertexs[1].TexCoord },
											TempVertexType{ _FaceVertexs[2].Location,_FaceVertexs[2].TexCoord });

			const vec3 Normal = Mesh::CalculateNormal(Tangent_BiNormal.first, Tangent_BiNormal.second);

			for (auto& _CurVertex : _FaceVertexs) 
			{ 
				_CurVertex.Normal = Normal; 
				_CurVertex.Tangent = Tangent_BiNormal.first; 
				_CurVertex.BiNormal = Tangent_BiNormal.second; 
			};

			_Vertexs.insert(std::end(_Vertexs),
				std::make_move_iterator(std::begin(_FaceVertexs)),
				std::make_move_iterator(std::end(_FaceVertexs)));

			ReplaceLine.clear();
		}

		if (ToKen.empty() && !_Vertexs.empty())
		{
			SubSetInfo _Info;

			if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(typename Vertex::Texture) *
				_Vertexs.size(),
				D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT,
				&_Info.VtxBuf, 0)))
			{
				MessageBox(nullptr, __FUNCTIONW__, nullptr, 0);
			}

			typename Vertex::Texture* _VtxBuffer;
			_Info.VtxBuf->Lock(0, 0, (void**)&_VtxBuffer, 0);

			memcpy(_VtxBuffer, _Vertexs.data(), sizeof(typename Vertex::Texture) * _Vertexs.size());
			_Info.TriangleCount = _Vertexs.size() / 3;
			// 충돌 정보 생성....
			
			for (auto iter = _Vertexs.begin(); iter != _Vertexs.end();)
			{
				PlaneInfo _PlaneInfo;

				_PlaneInfo.Face[0] = iter->Location;
				_PlaneInfo.Face[1] = (++iter)->Location;
				_PlaneInfo.Face[2] = (++iter)->Location;

				// 로컬에서 정의된 정점들을 월드로 바꿔서 저장
				_PlaneInfo.Center = { 0,0,0 };

				std::transform(std::make_move_iterator(std::begin(_PlaneInfo.Face)), 
					std::make_move_iterator(std::end(_PlaneInfo.Face)), 
					std::begin(_PlaneInfo.Face), [&MapWorld,&_PlaneInfo]
					(auto _VertexPoint)
					{
						D3DXVec3TransformCoord(&_VertexPoint,&_VertexPoint, &MapWorld);
						_PlaneInfo.Center += _VertexPoint;
						return _VertexPoint;
					});

				D3DXPlaneFromPoints(&_PlaneInfo._Plane,
					&_PlaneInfo.Face[0],&_PlaneInfo.Face[1], &_PlaneInfo.Face[2]);

				_PlaneInfo.Center /= 3.f;

				++iter;

				_PolygonPlanes.push_back(std::move(_PlaneInfo));
			}
			// 이제 같은 평면을 추려내자 .......
			//
			_Vertexs.clear();
			_Info.VtxBuf->Unlock();

			auto iter = _MtrlInfo.find(CurMtrlInfoName);

			if (iter == std::end(_MtrlInfo))
			{
				MessageBox(nullptr, L"	if (iter == std::end ( _MtrlInfo)  ) ", nullptr, 0);
			}
			else
			{
				_Info.MaterialInfo = iter->second;

				std::wstring TexName = FilePath + _Info.MaterialInfo.TextureName; 
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					TexName.c_str(), &_Info.Diffuse)))
				{
					MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}

				TexName.erase(TexName.find_last_of('.'));
				const std::wstring SpecularTexName = TexName + L"_SPECULAR.png";
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					SpecularTexName.c_str(), &_Info.Specular)))
				{
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}

				const std::wstring NormalTexName = TexName + L"_NORMAL.png";
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					NormalTexName.c_str(), &_Info.Normal)))
				{
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}
			}
			_Info.Decl =			Vertex::Texture::GetVertexDecl(m_pDevice);
			_WallSubSetInfo->push_back(_Info);
		}

		wss.str(std::wstring{});
		Line.clear();
		wss.clear();
	};

	CCollisionComponent::AddMapPlaneInfo(_PolygonPlanes);
};

void CMapBase::Free()
{
	// 2020.12.14 5:46 KMJ
	// 버텍스 버퍼, 텍스처 해제
	for (auto& pInfo : *_WallSubSetInfo) {
		SafeRelease(pInfo.Diffuse);
		SafeRelease(pInfo.Specular);
		SafeRelease(pInfo.Normal);
		SafeRelease(pInfo.VtxBuf);
		SafeRelease(pInfo.Decl);
		pInfo.Release();
	}
	for (auto& pFloorInfo : *_FloorSubSetInfo)
	{
		SafeRelease(pFloorInfo .Diffuse);
		SafeRelease(pFloorInfo .Specular);
		SafeRelease(pFloorInfo .Normal);
		SafeRelease(pFloorInfo .VtxBuf);
		SafeRelease(pFloorInfo.Decl);
		pFloorInfo.Release();
	}
	_WallSubSetInfo->clear();
	_FloorSubSetInfo->clear();
	Super::Free();
}
void CMapBase::CubeMapRender()
{
	mat View;
	m_pDevice->GetTransform(D3DTS_VIEW, &View);

	mat InvView=MATH::Inverse(View);
	mat CubeWorld = MATH::WorldMatrix({ 2.5,2.5,2.5 }, { 0,0,0 },{ InvView._41,InvView._42,InvView._43});
	m_pDevice->SetTransform(D3DTS_WORLD, &CubeWorld);

	DWORD _CullModePrev;
	DWORD _ZEnablePrev;
	DWORD _LightingPrev;
	DWORD _SampAddressU;
	DWORD _SampAddressV;

	m_pDevice->GetRenderState(D3DRS_CULLMODE, &_CullModePrev);
	m_pDevice->GetRenderState(D3DRS_ZENABLE, &_ZEnablePrev);
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &_LightingPrev);
	m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &_SampAddressU);
	m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &_SampAddressV);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
	m_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// 여기서 드로잉..
	{
		m_pDevice->SetStreamSource(0, _CubeVertexBuf.get(), 0, sizeof(Vertex::CubeTexture));
		m_pDevice->SetFVF(Vertex::CubeTexture::FVF);
		m_pDevice->SetIndices(_CubeIndexBuf.get());
		m_pDevice->SetTexture(0, _CubeTexture.get());
		m_pDevice->SetVertexShader(nullptr);
		m_pDevice->SetPixelShader(nullptr);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
			8, 0, 12);
	}

	m_pDevice->SetRenderState(D3DRS_CULLMODE,_CullModePrev);
	m_pDevice->SetRenderState(D3DRS_ZENABLE,_ZEnablePrev);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, _LightingPrev);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, _SampAddressU);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, _SampAddressV);
};

void CMapBase::LoadFloor(const std::wstring& FilePath)
{
	_FloorSubSetInfo = std::shared_ptr<std::vector<SubSetInfo>>(new std::vector<SubSetInfo>, []
	(auto& ptr)
		{
			for (auto& CurElement : *ptr)
			{
				CurElement.Release();
			};
			delete ptr;
		});

	std::vector<PlaneInfo> _PolygonPlanes;

	const std::wstring _MtlFileName = FilePath + L"Floor.mtl";
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
					_Info.Diffuse.a = 1.f;
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
					_Info.Specular.a = 1.f;
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

	std::wstring _ObjFileName = FilePath + L"Floor.obj";

	const std::wstring V = L"v";
	const std::wstring Vt = L"vt";
	const std::wstring VN = L"vn";
	const std::wstring F = L"f";
	const std::wstring Mtl = L"usemtl";
	const wchar_t FDelim = L'\/';
	size_t VtxElementCount = 3;

	std::wfstream _ObjStream(_ObjFileName);

	if (!_ObjStream.is_open())
		::MessageBox(nullptr, __FUNCTIONW__,
			L"File Open Fail", 0);

	std::vector<typename Vertex::Texture> _Vertexs;
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

			std::array<typename Vertex::Texture, 3ul> _FaceVertexs;

			for (size_t i = 0; i < 3; ++i)
			{
				_int LocationIdx, NormalIdx, TexCoordIdx;
				wss >> LocationIdx;
				wss >> TexCoordIdx;
				wss >> NormalIdx;
				typename Vertex::Texture _Vtx;
				_Vtx.Location = _Locations[LocationIdx - 1];
				_Vtx.Normal = _Normals[NormalIdx - 1];
				_Vtx.TexCoord = _TextureCoords[TexCoordIdx - 1];

				_FaceVertexs[i] = (std::move(_Vtx));
			};

			const auto Tangent_BiNormal = Mesh::CalculateTangentBinormal(
				TempVertexType{ _FaceVertexs[0].Location,_FaceVertexs[0].TexCoord },
				TempVertexType{ _FaceVertexs[1].Location,_FaceVertexs[1].TexCoord },
				TempVertexType{ _FaceVertexs[2].Location,_FaceVertexs[2].TexCoord });

			const vec3 Normal = Mesh::CalculateNormal(Tangent_BiNormal.first, Tangent_BiNormal.second);

			for (auto& _CurVertex : _FaceVertexs)
			{
				_CurVertex.Normal = Normal;
				_CurVertex.Tangent = Tangent_BiNormal.first;
				_CurVertex.BiNormal = Tangent_BiNormal.second;
			};

			_Vertexs.insert(std::end(_Vertexs),
				std::make_move_iterator(std::begin(_FaceVertexs)),
				std::make_move_iterator(std::end(_FaceVertexs)));

			ReplaceLine.clear();
		}

		if (ToKen.empty() && !_Vertexs.empty())
		{
			SubSetInfo _Info;

			if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(typename Vertex::Texture) *
				_Vertexs.size(),
				D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT,
				&_Info.VtxBuf, 0)))
			{
				MessageBox(nullptr, __FUNCTIONW__, nullptr, 0);
			}

			typename Vertex::Texture* _VtxBuffer;
			_Info.VtxBuf->Lock(0, 0, (void**)&_VtxBuffer, 0);

			memcpy(_VtxBuffer, _Vertexs.data(), sizeof(typename Vertex::Texture) * _Vertexs.size());
			_Info.TriangleCount = _Vertexs.size() / 3;
			// 충돌 정보 생성....

			for (auto iter = _Vertexs.begin(); iter != _Vertexs.end();)
			{
				PlaneInfo _PlaneInfo;

				_PlaneInfo.Face[0] = iter->Location;
				_PlaneInfo.Face[1] = (++iter)->Location;
				_PlaneInfo.Face[2] = (++iter)->Location;

				// 로컬에서 정의된 정점들을 월드로 바꿔서 저장
				_PlaneInfo.Center = { 0,0,0 };

				std::transform(std::make_move_iterator(std::begin(_PlaneInfo.Face)),
					std::make_move_iterator(std::end(_PlaneInfo.Face)),
					std::begin(_PlaneInfo.Face), [MapWorld = MapWorld, &_PlaneInfo]
					(auto _VertexPoint)
					{
						D3DXVec3TransformCoord(&_VertexPoint, &_VertexPoint, &MapWorld);
						_PlaneInfo.Center += _VertexPoint;
						return _VertexPoint;
					});

				D3DXPlaneFromPoints(&_PlaneInfo._Plane,
					&_PlaneInfo.Face[0], &_PlaneInfo.Face[1], &_PlaneInfo.Face[2]);

				_PlaneInfo.Center /= 3.f;

				++iter;

				_PolygonPlanes.push_back(std::move(_PlaneInfo));
			}
			// 이제 같은 평면을 추려내자 .......
			//
			_Vertexs.clear();
			_Info.VtxBuf->Unlock();

			auto iter = _MtrlInfo.find(CurMtrlInfoName);

			if (iter == std::end(_MtrlInfo))
			{
				MessageBox(nullptr, L"	if (iter == std::end ( _MtrlInfo)  ) ", nullptr, 0);
			}
			else
			{
				_Info.MaterialInfo = iter->second;

				std::wstring TexName = FilePath + _Info.MaterialInfo.TextureName;
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					TexName.c_str(), &_Info.Diffuse)))
				{
					MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}

				TexName.erase(TexName.find_last_of('.'));
				const std::wstring SpecularTexName = TexName + L"_SPECULAR.png";
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					SpecularTexName.c_str(), &_Info.Specular)))
				{
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}

				const std::wstring NormalTexName = TexName + L"_NORMAL.png";
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					NormalTexName.c_str(), &_Info.Normal)))
				{
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}
			}
			_Info.Decl = Vertex::Texture::GetVertexDecl(m_pDevice);
			_FloorSubSetInfo->push_back(_Info);
		}

		wss.str(std::wstring{});
		Line.clear();
		wss.clear();
	};

	CCollisionComponent::AddMapFloorInfo(_PolygonPlanes);
}
void CMapBase::LoadCubeMap(const std::wstring& FilePath)
{
	IDirect3DCubeTexture9* _CubeTexturePtr = nullptr;

	D3DXCreateCubeTextureFromFile(m_pDevice,
		FilePath.c_str(), &_CubeTexturePtr);

	assert(_CubeTexturePtr);

	_CubeTexture = std::shared_ptr<IDirect3DCubeTexture9>(_CubeTexturePtr,
		[](IDirect3DCubeTexture9* const _Target)
		{
			_Target->Release();
		});

	IDirect3DVertexBuffer9* _VertexBuf = nullptr;

	if (FAILED(m_pDevice->CreateVertexBuffer(
		sizeof(Vertex::CubeTexture) * 8,
		0,
		Vertex::CubeTexture::FVF,
		D3DPOOL_MANAGED, 
		&_VertexBuf, /* 할당된 버텍스버퍼의 주소를 반환 */
		nullptr)))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
	}

	Vertex::CubeTexture* CubeVertexPtr = nullptr;
	_VertexBuf->Lock(0, 0, (void**)&CubeVertexPtr, 0);

	CubeVertexPtr[0].UV = CubeVertexPtr[0].Location = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	CubeVertexPtr[1].UV = CubeVertexPtr[1].Location = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	CubeVertexPtr[2].UV = CubeVertexPtr[2].Location = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	CubeVertexPtr[3].UV = CubeVertexPtr[3].Location = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	CubeVertexPtr[4].UV = CubeVertexPtr[4].Location = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	CubeVertexPtr[5].UV = CubeVertexPtr[5].Location = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	CubeVertexPtr[6].UV = CubeVertexPtr[6].Location = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	CubeVertexPtr[7].UV = CubeVertexPtr[7].Location = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	_VertexBuf->Unlock();

	_CubeVertexBuf = std::shared_ptr<IDirect3DVertexBuffer9>
		(_VertexBuf,
			[](IDirect3DVertexBuffer9* const _Target)
			{
				_Target->Release();
			});


	IDirect3DIndexBuffer9* _IndexBuf = nullptr;
	/* 인덱스버퍼 생성 */
	if (FAILED(m_pDevice->CreateIndexBuffer(
		sizeof(Index::_16_t) * 12, /* 인덱스버퍼가 관리할 배열의 총 사이즈 */
		0, /* 0이면 정적버퍼 */
		Index::_16_t::Format, /* 인덱스 포맷 */
		D3DPOOL_MANAGED, /* 메모리 보관 방식 */
		&_IndexBuf,
		nullptr)))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
	}

	Index::_16_t* _IndexBufPtr = nullptr;
	_IndexBuf->Lock(0, 0, (void**)&_IndexBufPtr, 0);
		
	// +x
	_IndexBufPtr[0]._1 = 1;
	_IndexBufPtr[0]._2 = 5;
	_IndexBufPtr[0]._3 = 6;
	_IndexBufPtr[1]._1 = 1;
	_IndexBufPtr[1]._2 = 6;
	_IndexBufPtr[1]._3 = 2;
	_IndexBufPtr[2]._1 = 4;
	_IndexBufPtr[2]._2 = 0;
	_IndexBufPtr[2]._3 = 3;
	_IndexBufPtr[3]._1 = 4;
	_IndexBufPtr[3]._2 = 3;
	_IndexBufPtr[3]._3 = 7;
	_IndexBufPtr[4]._1 = 4;
	_IndexBufPtr[4]._2 = 5;
	_IndexBufPtr[4]._3 = 1;
	_IndexBufPtr[5]._1 = 4;
	_IndexBufPtr[5]._2 = 1;
	_IndexBufPtr[5]._3 = 0;
	_IndexBufPtr[6]._1 = 3;
	_IndexBufPtr[6]._2 = 2;
	_IndexBufPtr[6]._3 = 6;
	_IndexBufPtr[7]._1 = 3;
	_IndexBufPtr[7]._2 = 6;
	_IndexBufPtr[7]._3 = 7;
	_IndexBufPtr[8]._1 = 5;
	_IndexBufPtr[8]._2 = 4;
	_IndexBufPtr[8]._3 = 7;
	_IndexBufPtr[9]._1 = 5;
	_IndexBufPtr[9]._2 = 7;
	_IndexBufPtr[9]._3 = 6;
	_IndexBufPtr[10]._1 = 0;
	_IndexBufPtr[10]._2 = 1;
	_IndexBufPtr[10]._3 = 2;
	_IndexBufPtr[11]._1 = 0;
	_IndexBufPtr[11]._2 = 2;
	_IndexBufPtr[11]._3 = 3;

	_IndexBuf->Unlock();

	_CubeIndexBuf = std::shared_ptr<IDirect3DIndexBuffer9>(
			_IndexBuf, [](IDirect3DIndexBuffer9 * const _Target)
			{
				_Target->Release();
			} );
};

