#include "stdafx.h"
#include "MapBase.h"
#include "CollisionComponent.h"
#include "DXWrapper.h"
#include "ImGuiHelper.h"
#include "Stage.h"
#include "MainCamera.h"
#include "Player.h"

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
		ImGui::Begin("LightColor" ,& ImGuiHelper::bEditOn);
		ImGui::SliderFloat3("DiffuseColor", (float*)&diffusecolor, 0.0f, 1.0f);
		ImGui::End();
	}
	
	
	return _uint();
}

_uint CMapBase::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	m_pManagement->SetAmbient(MapAmbient);

	return _uint();
}

HRESULT CMapBase::RenderGameObject()
{
	mat CurrentWorld = this->MapWorld;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &CurrentWorld)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
	m_pDevice->SetRenderState(D3DRS_AMBIENT,MapAmbient);

	const vec4 CameraLocation =
		MATH::ConvertVec4(dynamic_cast<CStage*>
			(m_pManagement->GetCurrentScene())->_Camera->GetTransform()->GetLocation(), 1.f);

	const vec4 LightLocation = 
		MATH::ConvertVec4(dynamic_cast<CStage*>
			(m_pManagement->GetCurrentScene())->m_pPlayer->GetTransform()->GetLocation(), 1.f);

	mat View;
	mat Projection;

	m_pDevice->GetTransform(D3DTS_VIEW, &View);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &Projection);

	// 버텍스 
	{
		_ShaderInfo.VsTable->SetMatrix(m_pDevice, _ShaderInfo.VsHandleMap["World"],
			&CurrentWorld);
		_ShaderInfo.VsTable->SetMatrix(m_pDevice, _ShaderInfo.VsHandleMap["View"],
			&View);
		_ShaderInfo.VsTable->SetMatrix(m_pDevice, _ShaderInfo.VsHandleMap["Projection"],
			&Projection);
		_ShaderInfo.VsTable->SetVector(m_pDevice, _ShaderInfo.VsHandleMap["WorldLightLocation"],
			&LightLocation);
		_ShaderInfo.VsTable->SetVector(m_pDevice, _ShaderInfo.VsHandleMap["WorldCameraLocation"],
			&CameraLocation);

		m_pDevice->SetVertexShader(_ShaderInfo.VsShader);
	}

	{
		uint32_t count = 0;
		D3DXCONSTANT_DESC _Desc;
		
		_ShaderInfo.PsTable->SetVector(m_pDevice,_ShaderInfo.PsHandleMap["LightColor"],
		&diffusecolor);
		
		m_pDevice->SetPixelShader(_ShaderInfo.PsShader);
	}

	D3DCAPS9 _Caps;
	m_pDevice->GetDeviceCaps(&_Caps);
	for (auto& RefInfo : *_InfosPtr)
	{
	   {
	   		const uint32_t TexIdx = 
	   			_ShaderInfo.TextureDescMap["DiffuseSampler"].RegisterIndex;
	   		m_pDevice->SetTexture(TexIdx, RefInfo.Texture);
	   
	   		m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	   		m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	   		m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	   		m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MAXANISOTROPY, _Caps.MaxAnisotropy);
	   }

	   {
			const uint32_t TexIdx =
				_ShaderInfo.TextureDescMap["SpecularSampler"].RegisterIndex;
			m_pDevice->SetTexture(TexIdx, RefInfo.TextureSpecular);

			m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDevice->SetSamplerState(TexIdx, D3DSAMP_MAXANISOTROPY, _Caps.MaxAnisotropy);
		}

		m_pDevice->SetStreamSource(0, RefInfo.VtxBuf, 0, sizeof(Vertex));
		m_pDevice->SetVertexDeclaration(RefInfo.Decl);
		/*m_pDevice->SetVertexShader(nullptr);
		m_pDevice->SetPixelShader(nullptr);*/
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0,RefInfo.TriangleCount);
	}

	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	return S_OK;
}

void CMapBase::LoadMap(std::wstring FilePath,
	const mat& MapWorld)
{
	this->MapWorld = MapWorld;

	_InfosPtr = std::shared_ptr<std::vector<Info>>(new std::vector<Info>,
		[](auto ptr) 
		{
			for (auto& element : (*ptr) )
			{
				SafeRelease(element.VtxBuf);
				SafeRelease(element.Texture);
				SafeRelease(element.TextureSpecular);
				SafeRelease(element.Decl);
			}
			delete ptr;
		});

	_PolygonPlane = std::make_shared<std::vector<PlaneInfo>>();

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

	std::vector<Vertex> _Vtxs;
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

			std::array<Vertex,3ul> _FaceVertexs;

			for (size_t i = 0; i < 3; ++i)
			{
				_int LocationIdx, NormalIdx, TexCoordIdx;
				wss >> LocationIdx;
				wss >> TexCoordIdx;
				wss >> NormalIdx;
				Vertex _Vtx;
				_Vtx.Location = _Locations[LocationIdx - 1];
				_Vtx.Normal = _Normals[NormalIdx - 1];
				_Vtx.TexCoord = _TextureCoords[TexCoordIdx - 1];

				_FaceVertexs[i] = (std::move(_Vtx));
			};

			_Vtxs.insert(std::end(_Vtxs),
				std::make_move_iterator(std::begin(_FaceVertexs)),
				std::make_move_iterator(std::end(_FaceVertexs)));

			ReplaceLine.clear();
		}

		if (ToKen.empty() && !_Vtxs.empty())
		{
			Info _Info;

			if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(Vertex) *
				_Vtxs.size(),
				D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT,
				&_Info.VtxBuf, 0)))
			{
				MessageBox(nullptr, __FUNCTIONW__, nullptr, 0);
			}

			Vertex* _VtxBuffer;
			_Info.VtxBuf->Lock(0, 0, (void**)&_VtxBuffer, 0);

			memcpy(_VtxBuffer, _Vtxs.data(), sizeof(Vertex) * _Vtxs.size());
			_Info.TriangleCount = _Vtxs.size() / 3;
			// 충돌 정보 생성....
			
			for (auto iter = _Vtxs.begin(); iter != _Vtxs.end();)
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

				_PolygonPlane->push_back(std::move(_PlaneInfo));
			}
			// 이제 같은 평면을 추려내자 .......
			//
			_Vtxs.clear();
			_Info.VtxBuf->Unlock();

			auto iter = _MtrlInfo.find(CurMtrlInfoName);

			if (iter == std::end(_MtrlInfo))
			{
				MessageBox(nullptr, L"	if (iter == std::end ( _MtrlInfo)  ) ", nullptr, 0);
			}
			else
			{
				_Info.MaterialInfo = iter->second;

				std::wstring TexName = FilePath;
				TexName += _Info.MaterialInfo.TextureName;


				if (FAILED(D3DXCreateTextureFromFile(m_pDevice,
					TexName.c_str(), &_Info.Texture)))
				{
					MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
				}
			}
			_Info.Decl = Vertex::GetVertexDecl(m_pDevice);
			_InfosPtr->push_back(_Info);
		}

		wss.str(std::wstring{});
		Line.clear();
		wss.clear();
	};

	CCollisionComponent::SetUpMapPlaneInfo(*_PolygonPlane);
}

void CMapBase::Free()
{
	// 2020.12.14 5:46 KMJ
	// 버텍스 버퍼, 텍스처 해제
	for (auto& pInfo : *_InfosPtr) {
		SafeRelease(pInfo.Texture);
		SafeRelease(pInfo.VtxBuf);
	}
	_InfosPtr->clear();

	Super::Free();
}
