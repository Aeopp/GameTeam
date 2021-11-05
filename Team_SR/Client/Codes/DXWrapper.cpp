#include "stdafx.h"
#include "DXWrapper.h"
#include "Vertexs.h"


std::map<std::wstring, typename Effect::Info> Effect::_EffectInfoMap{ };
std::vector<MyLight> Effect::_CurMapLights;

D3DMATERIAL9 Light::GetMtrl(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, const float Power)
{
	D3DMATERIAL9 Mtl;
	::ZeroMemory(&Mtl, sizeof(Mtl));
	Mtl.Diffuse = std::move(Diffuse);
	Mtl.Ambient = std::move(Ambient);
	Mtl.Specular = std::move(Specular);
	Mtl.Emissive = std::move(Emissive);
	Mtl.Power = Power;
	return Mtl;
}

D3DLIGHT9 Light::GetDirectional(const vec3& Direction, const D3DXCOLOR& Color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = Color * 0.6f;
	light.Diffuse = Color;
	light.Specular = Color * 0.6f;
	light.Direction = Direction;

	return light;
}

D3DLIGHT9 Light::GetPoint(const vec3& Location, const D3DXCOLOR& Color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = Color * 0.6f;
	light.Diffuse = Color;
	light.Specular = Color * 0.6f;
	light.Position = Location;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 Light::GetSpot(const vec3& Location, const vec3& Direction, const D3DXCOLOR& Color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = Color * 0.0f;
	light.Diffuse = Color;
	light.Specular = Color * 0.6f;
	light.Position = Location;
	light.Direction = Direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.4f;
	light.Phi = 0.9f;

	return light;
};
;
std::pair<vec3, vec3> Mesh::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3)
{
	vec3 tangent, binormal;

	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];

	// 현재 표면의 두 벡터를 계산합니다.
	vector1[0] = vertex2.location.x - vertex1.location.x;
	vector1[1] = vertex2.location.y - vertex1.location.y;
	vector1[2] = vertex2.location.z - vertex1.location.z;

	vector2[0] = vertex3.location.x - vertex1.location.x;
	vector2[1] = vertex3.location.y - vertex1.location.y;
	vector2[2] = vertex3.location.z - vertex1.location.z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	tuVector[0] = vertex2.TexCoord.x - vertex1.TexCoord.x;
	tvVector[0] = vertex2.TexCoord.y - vertex1.TexCoord.y;

	tuVector[1] = vertex3.TexCoord.x - vertex1.TexCoord.x;
	tvVector[1] = vertex3.TexCoord.y - vertex1.TexCoord.y;

	// 탄젠트 / 바이 노멀 방정식의 분모를 계산합니다.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// 법선을 표준화 한 다음 저장합니다.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// 이 법선의 길이를 계산합니다.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// 법선을 표준화 한 다음 저장합니다.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return std::pair<vec3, vec3>{tangent,binormal};
}

vec3 Mesh::CalculateNormal(const vec3 Tangent,const vec3 BiNormal)
{
	vec3 Normal;

	// 법선 벡터를 줄 수있는 접선과 binormal의 외적을 계산합니다.
	Normal.x = (Tangent.y * BiNormal.z) - (Tangent.z * BiNormal.y);
	Normal.y = (Tangent.z * BiNormal.x) - (Tangent.x * BiNormal.z);
	Normal.z = (Tangent.x * BiNormal.y) - (Tangent.y * BiNormal.x);

	// 법선의 길이를 계산.
	float length = sqrt((Normal.x * Normal.x) + (Normal.y * Normal.y) + (Normal.z * Normal.z));

	// 법선을 노멀라이즈.
	Normal.x = Normal.x / length;
	Normal.y = Normal.y / length;
	Normal.z = Normal.z / length;

	return Normal;
};

// 조명 등록은 Update 타이밍에 .
void Effect::RegistLight(MyLight _Light) noexcept
{
	_CurMapLights.push_back(std::move(_Light));
}

void Effect::ClearRegisteredLighting() noexcept
{
	_CurMapLights.clear();
}

typename Effect::Info& Effect::GetEffectFromName(const std::wstring& EffectName)
{
#if _DEBUG
	if (EffectName.empty() || _EffectInfoMap.find(EffectName) == std::end(_EffectInfoMap))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return _EffectInfoMap[EffectName];
	}
#endif
	return _EffectInfoMap[EffectName];
}

void Effect::EffectRelease()
{
	for (auto& _CurEffect : _EffectInfoMap)
	{
		SafeRelease(_CurEffect.second.VsTable);
		SafeRelease(_CurEffect.second.PsTable);
		SafeRelease(_CurEffect.second.VsShader);
		SafeRelease(_CurEffect.second.PsShader);
	}
}

void Effect::EffectInitialize(IDirect3DDevice9* const _Device)
{
	{
		// 디퓨즈 + 스페큘러 ..........
		Effect::Info _EffectInfo = Effect::CompileAndCreate
		(_Device, L"..\\Shader\\DiffuseSpecular");

		_EffectInfo.VsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.VsTable,
			std::vector<std::string>{
				"World",
				"View",
				"Projection",
				"WorldCameraLocation",
			    "UVFlag",
				"bUI"
		});

		_EffectInfo.PsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.PsTable,
			std::vector<std::string>{
				"LightNum",
				"Shine",
				"GlobalAmbient",
				"LightLocation",
				"LightRadius",
				"LightDiffuse",
				"bSpecularSamplerBind",
				"bNormalSamplerBind",
				"bUVAlphaLerp",
				 "FogEnd",
				 "FogStart",
				 "FogColor",
				"bUI",
				"ColorLerpT",
				"LightCalcFlag",

		});

		_EffectInfo.TextureDescMap = Effect::ConstantHandleDescInitialize
		(_EffectInfo.PsTable,
			{ "DiffuseSampler",
			  "SpecularSampler",
			  "NormalSampler",
			  "EnvironmentSampler"});

		_EffectInfoMap[L"DiffuseSpecular"] = std::move(_EffectInfo);
	}

	{
		// For MiniMap
		Effect::Info _EffectInfo = Effect::CompileAndCreate
		(_Device, L"..\\Shader\\MiniMap");

		_EffectInfo.VsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.VsTable,
			std::vector<std::string>
		{
			"World",
			"Projection",
			
		});

		_EffectInfo.PsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.PsTable,
			std::vector<std::string>
		{
			"PlayerScreenLocation",
			"DistanceMin",
			"Relative"
		});

		_EffectInfo.TextureDescMap = Effect::ConstantHandleDescInitialize
		(_EffectInfo.PsTable,
			{ 
			 
			});

		_EffectInfoMap[L"MiniMap"] = std::move(_EffectInfo);
	}



	{
		Effect::Info _EffectInfo = Effect::CompileAndCreate
		(_Device, L"..\\Shader\\UITexture");

		_EffectInfo.VsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.VsTable,
			std::vector<std::string>
		{
				"World",
				"Projection",
		});

		_EffectInfo.PsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.PsTable,
			std::vector<std::string>
		{
				"RenderFlag",
				"ColorCoefft",
		});

		_EffectInfo.TextureDescMap = Effect::ConstantHandleDescInitialize
		(_EffectInfo.PsTable,
			{ 
			  "DiffuseSampler",
			});

		_EffectInfoMap[L"UITexture"] = std::move(_EffectInfo);
	};

	{
		Effect::Info _EffectInfo = Effect::CompileAndCreate
		(_Device, L"..\\Shader\\ScreenPostEffect");

		_EffectInfo.VsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.VsTable,
			std::vector<std::string>
		{
				"World",
				"Projection",
		});

		_EffectInfo.PsHandleMap = Effect::ConstantHandleInitialize(
			_EffectInfo.PsTable,
			std::vector<std::string>
		{
			"AlphaCoefft",
			"Flag",
		});

		_EffectInfo.TextureDescMap = Effect::ConstantHandleDescInitialize
		(_EffectInfo.PsTable,
			{
			  "DiffuseSampler",
			});

		_EffectInfoMap[L"ScreenPostEffect"] = std::move(_EffectInfo);
	};

}


void Effect::Update(IDirect3DDevice9* const _Device, const vec4& CameraLocation,
	const vec4& LightLocation)
{
	mat View;
	mat Projection;
	_Device->GetTransform(D3DTS_VIEW, &View);
	_Device->GetTransform(D3DTS_PROJECTION, &Projection);

	const vec3 CameraLocation3 = CameraLocation;
	const size_t MapLightSize = _CurMapLights.size(); 

	std::vector<vec4> LightLocations;
	std::vector<vec4> LightDiffuse;
	std::vector<float> LightRadius;

	// 1. 우선 순위에 의해 정렬하고 이후 거리가 가까운 조명으로 또다시 정렬
	// 미리 정의된 수의 조명만 사용 할 수 있기 때문에
	std::sort(std::begin(_CurMapLights), std::end(_CurMapLights), [](const MyLight&  Lhs , const MyLight& Rhs)
		{
			return Lhs.Priority < Rhs.Priority; 
		});

	std::stable_sort(std::begin(_CurMapLights) ,std::end(_CurMapLights) , [CameraLocation](const MyLight& Lhs, const MyLight& Rhs)
		{
			return MATH::LengthSq((CameraLocation - Lhs.Location)) < MATH::LengthSq((CameraLocation - Rhs.Location));
		});

	for (const MyLight&  _CurLight :_CurMapLights)
	{
		LightLocations.push_back(_CurLight.Location);
		LightDiffuse.push_back(_CurLight.Diffuse);
		LightRadius.push_back(_CurLight.Radius);
	}

	{
		Effect::Info& CurEffect = Effect::GetEffectFromName(L"DiffuseSpecular");
		CurEffect.SetVSConstantData(_Device, "View", View);
		CurEffect.SetVSConstantData(_Device, "Projection", Projection);
		CurEffect.SetVSConstantData(_Device, "WorldCameraLocation", CameraLocation);
		CurEffect.SetVSConstantData(_Device, "UVFlag", int32_t(0l));
		CurEffect.SetVSConstantData(_Device, "bUI", 0l);

		// 다중 조명시 수정 해야함...
		const vec4 GlobalAmbient = { 0.1f,0.1f,0.1f,1.f };
		 vec4 FogColor = { 0.7f,0.7f,0.7f,        1.f };

		if (static_cast<int32_t>(ESceneID::StageMidBoss) == CManagement::Get_Instance()->GetCurrentSceneIndex()
			||
			static_cast<int32_t>(ESceneID::StageFinalBoss) == CManagement::Get_Instance()->GetCurrentSceneIndex())
		{
			FogColor = {1.f,0.f,0.f,1.f};
		};

		CurEffect.SetPSConstantData(_Device, "GlobalAmbient", GlobalAmbient);
		CurEffect.SetPSConstantData(_Device, "FogStart", 1.f);
		CurEffect.SetPSConstantData(_Device, "FogEnd", 300.f);
		CurEffect.SetPSConstantData(_Device, "FogColor", FogColor);
		CurEffect.SetPSConstantData(_Device, "bUVAlphaLerp", 0l);
		CurEffect.SetPSConstantData(_Device, "bUI", 0l);
		CurEffect.SetPSConstantData(_Device, "ColorLerpT", 0.0f);
		

		CurEffect.SetPSConstantData(_Device, "LightNum", MapLightSize);
		CurEffect.PsTable->SetVectorArray(_Device, CurEffect.GetPSConstantHandle("LightLocation"),LightLocations.data(),LightLocations.size());
		CurEffect.PsTable->SetVectorArray(_Device, CurEffect.GetPSConstantHandle("LightDiffuse"), LightDiffuse.data(), LightDiffuse.size());
		CurEffect.PsTable->SetFloatArray(_Device, CurEffect.GetPSConstantHandle("LightRadius"), LightRadius.data(), LightRadius.size());
	}
}
std::map<std::string, D3DXHANDLE> Effect::ConstantHandleInitialize(
	ID3DXConstantTable* _ConstantTable,
	const std::vector<std::string>& _ConstantDataNames)
{
	std::map<std::string, D3DXHANDLE> ConstantHandleMap;

	for (const std::string& _ConstantDataName : _ConstantDataNames)
	{
		ConstantHandleMap[_ConstantDataName ] = 
		_ConstantTable->GetConstantByName(0, _ConstantDataName.c_str());
	}

	return ConstantHandleMap;
}

std::map<std::string, D3DXCONSTANT_DESC> Effect::ConstantHandleDescInitialize(
	ID3DXConstantTable* _ConstantTable, 
	const std::vector<std::string>& _ConstantTextureNames)
{
	std::map<std::string, D3DXCONSTANT_DESC> _ConstantDescMap;

	uint32_t count = 0;
	for (auto& _ConstantTextureName : _ConstantTextureNames)
	{
		auto TexHandle = _ConstantTable->GetConstantByName(0, _ConstantTextureName.c_str());
		if (!TexHandle) {PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);};
		D3DXCONSTANT_DESC _Desc;
		_ConstantTable->GetConstantDesc(TexHandle, &_Desc, &count);
		_ConstantDescMap[_ConstantTextureName] = (_Desc);
	};

	return _ConstantDescMap;
}

typename Effect::Info Effect::CompileAndCreate(
	IDirect3DDevice9* _Device,
	const std::wstring& FileName)
{
	Info _Info;

	// 버텍스
	{
		const std::wstring VsFileName = FileName + L"VS.hlsl";

		ID3DXBuffer* shader = 0;
		ID3DXBuffer* errorBuffer = 0;
		HRESULT hr;

		hr = D3DXCompileShaderFromFile(
			VsFileName.c_str(),
			0,
			0,
			"main", // entry point function name
			"vs_3_0",
			D3DXSHADER_DEBUG,
			&shader,
			&errorBuffer,
			&_Info.VsTable);

		// output any error messages
		if (errorBuffer)
		{
			//::MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
			SafeRelease(errorBuffer);
		}

		if (FAILED(hr))
		{
			::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		}

		//
		// Create shader
		//

		hr = _Device->CreateVertexShader(
			(DWORD*)shader->GetBufferPointer(),
			&_Info.VsShader);

		if (FAILED(hr))
		{
			::MessageBoxA(0, (char*)"CreateVertexShader - FAILED", 0, 0);
		}

		SafeRelease(shader);

		_Info.VsTable->SetDefaults(_Device);
	}

	// 픽셀
	{
		_Info.PsTable = nullptr;

		ID3DXBuffer* shader = 0;
		ID3DXBuffer* errorBuffer = 0;
		HRESULT hr;
		const std::wstring PsFileName = FileName + L"PS.hlsl";
		hr = D3DXCompileShaderFromFile(
			PsFileName.c_str(),
			0,
			0,
			"main", // entry point function name
			"ps_3_0",
			D3DXSHADER_DEBUG,
			&shader,
			&errorBuffer,
			&_Info.PsTable);

		// output any error messages
		if (errorBuffer)
		{
			//::MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
			SafeRelease(errorBuffer);
		}

		if (FAILED(hr))
		{
			::MessageBoxA(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
		}

		//
		// Create Pixel Shader
		//
		hr = _Device->CreatePixelShader(
			(DWORD*)shader->GetBufferPointer(),
			&_Info.PsShader);

		if (FAILED(hr))
		{
			::MessageBoxA(0, "CreateVertexShader - FAILED", 0, 0);
		}

		SafeRelease(shader);

		_Info.PsTable->SetDefaults(_Device);
	}

	return _Info;
}




void Effect::Info::Release()
{
	SafeRelease(PsShader);
	SafeRelease(VsShader);
	SafeRelease(VsTable);
	SafeRelease(PsTable);
}

void Effect::Info::AddRef()
{
	SafeAddRef(VsTable);
	SafeAddRef(PsTable);
	SafeAddRef(PsShader);
	SafeAddRef(VsShader);
}

uint8_t Effect::Info::GetTexIdx(const std::string& SamplerName)
{
#if _DEBUG
	if (SamplerName.empty() || TextureDescMap.find(SamplerName) == std::end(TextureDescMap))
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
#endif
	return TextureDescMap[SamplerName.c_str()].RegisterIndex;
}

D3DXHANDLE Effect::Info::GetVSConstantHandle(const std::string& HandleKey)
{
	auto iter = VsHandleMap.find(HandleKey);
	assert(iter != std::end(VsHandleMap));
	return iter->second;
}

D3DXHANDLE Effect::Info::GetPSConstantHandle(const std::string& HandleKey)
{
	auto iter = PsHandleMap.find(HandleKey);
	assert(iter != std::end(PsHandleMap));
	return iter->second;
}



std::vector<IDirect3DTexture9*> CreateTextures(IDirect3DDevice9* const _Device, const std::wstring& Path, const size_t TextureNum)
{
	std::vector<IDirect3DTexture9*> _TextureVec;

	for (size_t i = 0; i < TextureNum; ++i)
	{
		const std::wstring CurrentTextureFileName = Path + std::to_wstring(i) + L".png";
		IDirect3DTexture9* _Texture;

		if (FAILED(D3DXCreateTextureFromFile(_Device,
			CurrentTextureFileName.c_str(), &_Texture)))
		{
			PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		}
		else
		{
			_TextureVec.push_back(_Texture);
		}
	}

	return _TextureVec;
}

std::vector<std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*> >
CreateTexturesSpecularNormal(IDirect3DDevice9* const _Device, const std::wstring& Path, const size_t TextureNum)
{
	std::vector<std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*> > _TextureTupleVec(TextureNum);

	auto _Diffuse = CreateTextures(_Device, Path, TextureNum);
	auto _Specular= CreateTextures(_Device, Path+  L"Specular\\", TextureNum);
	auto _Normal =  CreateTextures(_Device, Path + L"Normal\\", TextureNum);

	for (size_t i = 0; i < TextureNum; ++i)
	{
		_TextureTupleVec[i] = std::make_tuple(
			std::move(_Diffuse[i]),
			std::move(_Specular[i]),
			std::move(_Normal[i]));
	};

	return _TextureTupleVec;
}

LPDIRECT3DTEXTURE9 LOAD_TEXTURE(IDirect3DDevice9* _Device, const std::wstring& FileName)
{
	LPDIRECT3DTEXTURE9 ret = nullptr;

	if (FAILED(D3DXCreateTextureFromFile(_Device, FileName.c_str(), &ret)))
	{
		OutputDebugString(L"텍스쳐 로딩 실패!!");
		OutputDebugString(FileName.c_str());
		OutputDebugString(L"\n");
	};

	return ret;
};

std::shared_ptr<std::vector<SubSetInfo>>  SubSetInfo::GetMeshFromObjFile(IDirect3DDevice9* const _Device,const std::wstring& FilePath)noexcept
{
	auto _SubsetInfo = std::shared_ptr<std::vector<SubSetInfo>>(new std::vector<SubSetInfo>{}, []
	(std::vector<SubSetInfo>* ptr)
		{
			for (auto& CurElement : *ptr)
			{
				CurElement.Release();
			};
			delete ptr;
		});

	const std::wstring _MtlFileName = FilePath + L".mtl";
	std::wfstream _MtlStream(_MtlFileName);
	if (!_MtlStream.is_open())
	{
		//::MessageBox(nullptr, __FUNCTIONW__,L"File Open Fail", 0);
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
					_MtrlInfo.insert({_Info.MtrlName  , _Info});
				}
			}
		}
	}

	std::wstring _ObjFileName = FilePath + L".obj";

	const std::wstring V = L"v";
	const std::wstring Vt = L"vt";
	const std::wstring VN = L"vn";
	const std::wstring F = L"f";
	const std::wstring Mtl = L"usemtl";
	const wchar_t FDelim = L'/';
	const size_t VtxElementCount = 3;

	std::wfstream _ObjStream(_ObjFileName);

	if (!_ObjStream.is_open())
	{
		::MessageBox(nullptr, __FUNCTIONW__, L"File Open Fail", 0);
	}

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
			//VertexTexCoord.y += 0.001f;
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

			if (FAILED(_Device->CreateVertexBuffer(sizeof(typename Vertex::Texture) *
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
			_Vertexs.clear();
			_Info.VtxBuf->Unlock();

			auto iter = _MtrlInfo.find(CurMtrlInfoName);

			if (iter == std::end(_MtrlInfo))
			{
				//MessageBox(nullptr, L"	if (iter == std::end ( _MtrlInfo)  ) ", nullptr, 0);
			}
			else
			{
				_Info.MaterialInfo = iter->second;
				const std::wstring TexName = FilePath + _Info.MaterialInfo.TextureName;
				if (FAILED(D3DXCreateTextureFromFile(_Device,
					TexName.c_str(), &_Info.Diffuse)))
				{
					#ifdef _DEBUG
					MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
					#endif
				}

				const std::wstring SpecularTexName = TexName + L"_SPECULAR";
				if (FAILED(D3DXCreateTextureFromFile(_Device,
					SpecularTexName.c_str(), &_Info.Specular)))
				{
					#ifdef _DEBUG
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
					#endif // _DEBUG
				}

				const std::wstring NormalTexName = TexName + L"_NORMAL";
				if (FAILED(D3DXCreateTextureFromFile(_Device,
					NormalTexName.c_str(), &_Info.Normal)))
				{
					#ifdef _DEBUG
					//MessageBox(nullptr, L"FAILED D3DXCreateTextureFromFile ", nullptr, 0);
					#endif // _DEBUG
				}
			}
			_Info.Decl = Vertex::Texture::GetVertexDecl(_Device);
			_SubsetInfo->push_back(_Info);
		}

		wss.str(std::wstring{});
		Line.clear();
		wss.clear();
	};

	return _SubsetInfo;
};

std::shared_ptr<SubSetInfo> SubSetInfo::MakeShared() noexcept
{
	return std::shared_ptr<SubSetInfo>(new SubSetInfo{},
		[](SubSetInfo* ptr)
		{
			ptr->Release();
			delete ptr;
		});
}

void SubSetInfo::Release() & noexcept
{
	SafeRelease(Decl);
	SafeRelease(VtxBuf);
	SafeRelease(Diffuse);
	SafeRelease(Specular);
	SafeRelease(Normal);
}

const std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*>& AnimationTextures::GetTexture(const std::wstring& _AnimKey, const size_t _ImgFrame)
{
	auto find_iter = _TextureMap.find(_AnimKey);

	if (find_iter == std::end(_TextureMap))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return {nullptr,nullptr ,nullptr };
	}
	else
	{
		return find_iter->second[_ImgFrame];
	}
}

void AnimationTextures::Release() & noexcept
{
	for (auto& _TexturePair: _TextureMap)
	{
		for (auto& _TextureTuple : _TexturePair.second)
		{
			std::get<0>(_TextureTuple)->Release();
			std::get<1>(_TextureTuple)->Release();
			std::get<2>(_TextureTuple)->Release();
		};
	}
}

void AnimationTextures::AddRef() & noexcept
{
	for (auto& _TexturePair : _TextureMap)
	{
		for (auto& _TextureTuple : _TexturePair.second)
		{
			std::get<0>(_TextureTuple)->AddRef();
			std::get<1>(_TextureTuple)->AddRef();
			std::get<2>(_TextureTuple)->AddRef();
		};
	}
}

void AnimationTextures::Update(const float DeltaTime)
{
	CurrentT += DeltaTime;

	if (CurrentT >= AnimDelta)
	{
		CurrentT -=AnimDelta;
		++CurrentImgFrame;

		auto find_iter = CurrentAnimNotify.find(CurrentImgFrame);
		if (find_iter != std::end(CurrentAnimNotify))
		{
			if(find_iter->second)
				find_iter->second();
		}

		if (CurrentImgFrame >= ImgNum)
		{
			if (bLoop)
			{
				CurrentImgFrame = 0;
			}
			else
			{
				CurrentImgFrame = (ImgNum - 1);
			}
		}
	}
}

typename AnimationTextures::AnimationInfo AnimationTextures::GetAnimInfo() const&
{
	AnimationInfo _Info;
	_Info.CurrentAnimKey = CurrentAnimKey;
	_Info.CurrentAnimNotify = CurrentAnimNotify;
	_Info.AnimDelta = AnimDelta;
	_Info.CurrentImgFrame = CurrentImgFrame;
	_Info.CurrentT = CurrentT;
	_Info.ImgNum = ImgNum;
	_Info.bLoop = bLoop;

	return _Info;
}

const std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*>& AnimationTextures::GetCurrentTexture()
{
	auto find_iter = _TextureMap.find(CurrentAnimKey);

	if (find_iter == std::end(_TextureMap))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return  { nullptr,nullptr,nullptr };
	}
	else
	{
		return find_iter->second[CurrentImgFrame];
	}
}

void AnimationTextures::ChangeAnim(
	std::wstring AnimKey, 
	const float AnimDelta, 
	const size_t ImgNum,
	const bool bLoop, 
	NotifyType  SetAnimNotify ,
	const float InitT, 
	const size_t StartImgFrame)
{
	CurrentAnimKey = std::move(AnimKey);
	this->AnimDelta = AnimDelta;
	CurrentT = InitT;
	CurrentImgFrame=StartImgFrame;
	this->ImgNum = ImgNum;
	this->bLoop = bLoop;
	this->CurrentAnimNotify = std::move(SetAnimNotify);
}
