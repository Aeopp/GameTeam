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
