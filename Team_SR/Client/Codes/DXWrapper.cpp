#include "stdafx.h"
#include "DXWrapper.h"


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

LPD3DXEFFECT Effect::LoadShader(IDirect3DDevice9* _Device, const std::wstring& FileName)
{
	LPD3DXEFFECT ret = nullptr;
	LPD3DXBUFFER Error = nullptr;
	uint32_t ShaderFlags = 0;

#if _DEBUG
	ShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(_Device, FileName.c_str(),
		nullptr, nullptr, ShaderFlags, nullptr, &ret, &Error);

	if (!ret && Error)
	{
		int Size = Error->GetBufferSize();
		void* ack = Error->GetBufferPointer();

		if (ack)
		{
			wchar_t* str = new wchar_t[Size];
			wsprintf(str, (const wchar_t*)ack, Size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

LPDIRECT3DTEXTURE9 Effect::LoadTexture(IDirect3DDevice9* _Device, const std::wstring& FileName)
{
	LPDIRECT3DTEXTURE9 ret = nullptr;
	if (FAILED(D3DXCreateTextureFromFile(_Device, FileName.c_str(), &ret)))
	{
		OutputDebugString(L"텍스쳐 로딩 실패!!");
		OutputDebugString(FileName.c_str());
		OutputDebugString(L"\n");
	};

	return ret;
}


std::pair<vec3, vec3> Model::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3)
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

vec3 Model::CalculateNormal(const vec3 Tangent,const vec3 BiNormal)
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

std::map<std::string, D3DXHANDLE> Shader::ConstantHandleInitialize( 
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

std::map<std::string, D3DXCONSTANT_DESC> Shader::ConstantHandleDescInitialize(
	ID3DXConstantTable* _ConstantTable, 
	const std::vector<std::string>& _ConstantTextureNames)
{
	std::map<std::string, D3DXCONSTANT_DESC> _ConstantDescMap;

	uint32_t count = 0;
	for (auto& _ConstantTextureName : _ConstantTextureNames)
	{
		auto TexHandle = _ConstantTable->GetConstantByName(0, _ConstantTextureName.c_str());
		D3DXCONSTANT_DESC _Desc;
		_ConstantTable->GetConstantDesc(TexHandle, &_Desc, &count);
		_ConstantDescMap[_ConstantTextureName] = (_Desc);
	};

	return _ConstantDescMap;
}

typename Shader::Info Shader::CompileAndCreate(IDirect3DDevice9* _Device, const std::wstring& FileName)
{
	Info _ShaderInfo;

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
			&_ShaderInfo.VsTable);

		// output any error messages
		if (errorBuffer)
		{
			::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
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
			&_ShaderInfo.VsShader);

		if (FAILED(hr))
		{
			::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		}

		SafeRelease(shader);

		_ShaderInfo.VsTable->SetDefaults(_Device);
	}

	// 픽셀
	{
		_ShaderInfo.PsTable = nullptr;

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
			&_ShaderInfo.PsTable);

		// output any error messages
		if (errorBuffer)
		{
			::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
			SafeRelease(errorBuffer);
		}

		if (FAILED(hr))
		{
			::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		}

		//
		// Create Pixel Shader
		//
		hr = _Device->CreatePixelShader(
			(DWORD*)shader->GetBufferPointer(),
			&_ShaderInfo.PsShader);

		if (FAILED(hr))
		{
			::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		}

		SafeRelease(shader);

		_ShaderInfo.PsTable->SetDefaults(_Device);
	}
	

	return _ShaderInfo;
}

