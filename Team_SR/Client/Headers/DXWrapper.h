#pragma once
#ifndef __DXWRAPPER_H__

#include "Engine_Include.h"

USING(Engine)

namespace Shader
{
	struct Info
	{
		IDirect3DPixelShader9* PsShader{ nullptr };
		ID3DXConstantTable* PsTable{ nullptr };
		IDirect3DVertexShader9* VsShader{ nullptr };
		ID3DXConstantTable* VsTable{ nullptr };
		/*ConstantHandleInitialize 로 세팅하면 편함.*/
		std::map<std::string, D3DXHANDLE> VsHandleMap;
		std::map<std::string, D3DXHANDLE> PsHandleMap;
		std::map<std::string, D3DXCONSTANT_DESC> TextureDescMap;

		void Release()
		{
			SafeRelease(PsShader);
			SafeRelease(VsShader);
		}
		void AddRef()
		{
			SafeAddRef(PsShader);
			SafeAddRef(VsShader);
		}
	};

	std::map<std::string, D3DXHANDLE> ConstantHandleInitialize(
		ID3DXConstantTable* _ConstantTable,
		const std::vector<std::string>& _ConstantDataNames);

	std::map<std::string, D3DXCONSTANT_DESC > ConstantHandleDescInitialize(
		ID3DXConstantTable* _ConstantTable,
		const std::vector<std::string>& _ConstantTextureNames
	);


	// 파일명 hlsl 확장자 없이 파일명만 입력
	// 쉐이더는 파일명+VS or PS 형식으로 제한
	Info CompileAndCreate(IDirect3DDevice9* _Device,
		const std::wstring& FileName);
}


namespace Effect
{
	LPD3DXEFFECT LoadShader(IDirect3DDevice9* _Device, const std::wstring& FileName);
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* _Device, const std::wstring& FileName);
}

struct TempVertexType
{
	vec3 location;
	vec3 TexCoord;
};

namespace Model
{
	// 삼각형 3 버텍스 로부터 탄젠트 바이노멀 벡터를 계산.
	std::pair<vec3,vec3/* 1st Tangent, 2nd BiNormal*/> 
		CalculateTangentBinormal(TempVertexType vertex1,
		TempVertexType vertex2, TempVertexType vertex3);
	// 탄젠트와 바이노멀로 새로운 노멀을 계산.
	vec3 CalculateNormal(const vec3 Tangent, const vec3 BiNormal);
}

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