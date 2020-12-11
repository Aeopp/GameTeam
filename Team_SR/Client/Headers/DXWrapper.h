#pragma once
#ifndef __DXWRAPPER_H__

#include "Engine_Include.h"

USING(Engine)

namespace Color
{
	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
};

namespace Light
{
	/// <summary>
	/// "Diffuse"><난반사광의 양>
	/// "Ambient"><환경광의 양>
	/// "Specular"><정반사광의 양>
	/// "Emissive"><전반적인 표면의 컬러(물체자체가 발광하는 경우)>
	/// "Power"><정반사광의 날카로운 정도(하이라이트가 강조)>
	D3DMATERIAL9 GetMtrl(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular,
		D3DXCOLOR Emissive, const float Power);

	const D3DMATERIAL9 WHITE_MTRL = GetMtrl(Color::WHITE, Color::WHITE, Color::WHITE, Color::BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = GetMtrl(Color::RED, Color::RED, Color::RED, Color::BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = GetMtrl(Color::GREEN, Color::GREEN, Color::GREEN, Color::BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = GetMtrl(Color::BLUE, Color::BLUE, Color::BLUE, Color::BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = GetMtrl(Color::YELLOW, Color::YELLOW, Color::YELLOW, Color::BLACK, 2.0f);

	D3DLIGHT9 GetDirectional(const vec3& Direction,const D3DXCOLOR& Color);
	D3DLIGHT9 GetPoint(const vec3& Location, const D3DXCOLOR& Color);
	D3DLIGHT9 GetSpot(const vec3& Location,const vec3& Direction,const D3DXCOLOR & Color);
};
#define __DXWRAPPER_H__
#endif