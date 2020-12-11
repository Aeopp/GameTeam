#pragma once
#ifndef __MapBase_H__

#include "GameObject.h"

USING(Engine)
class CMapBase  abstract : public CGameObject
{
protected:
	explicit CMapBase (LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMapBase () = default;
public:
	using Super = CGameObject;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
protected:
	void LoadMap(std::wstring FilePath,const mat& MapWorld);
public:
	/*static CMapBase * Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;*/
	virtual void Free() override;
	struct Vertex
	{
		vec3 Location;
		vec3 Normal;
		vec2 TexCoord;
		static const DWORD FVF;
	};

	struct MtrlInfo
	{
		enum class Illumination :uint8_t
		{

		};
		D3DXCOLOR Ambient{ 0,0,0,1 };
		D3DXCOLOR Diffuse = { 0,0,0,1 };
		D3DXCOLOR Specular{ 0,0,0,1 };
		// 광택 
		float Shine{ 0 };
		std::wstring TextureName{};
		std::wstring MtrlName{};
		Illumination Illu;

		D3DMATERIAL9 ConvertMtrl()
		{
			D3DMATERIAL9 _Mtrl;
			_Mtrl.Ambient = this->Ambient;
			_Mtrl.Diffuse = this->Diffuse;
			_Mtrl.Power = this->Shine;
			_Mtrl.Specular = Specular;

			return _Mtrl;
		}
	};

	struct Info
	{
		uint32_t TriangleCount = 0;
		IDirect3DVertexBuffer9* VtxBuf;
		MtrlInfo MaterialInfo;
		IDirect3DTexture9* Texture;
	};
protected:
	std::shared_ptr<std::vector<Info>> _InfosPtr;
	std::shared_ptr<std::vector<PlaneInfo>> _PolygonPlane;
	mat MapWorld;
	DWORD MapAmbient = 0x00202020;
	/*  Ex) ..\\Resources\\Map\\0\\ */
};

#define __MapBase_H__
#endif