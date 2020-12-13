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

LPD3DXEFFECT LoadShader(IDirect3DDevice9* _Device, const std::wstring& FileName)
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

LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* _Device, const std::wstring& FileName)
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
