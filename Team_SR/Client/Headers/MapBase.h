#pragma once
#ifndef __MapBase_H__

#include "GameObject.h"
#include "DXWrapper.h"

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
		vec3 Tanget;
		vec3 BiNormal;
		vec2 TexCoord;

		static LPDIRECT3DVERTEXDECLARATION9 GetVertexDecl( IDirect3DDevice9* _Device)
		{
			LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration;
			int count = 0;
			D3DVERTEXELEMENT9 decl[] =
			{
			  {0,  count,             D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			  {0, (12), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			  {0, (24), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
			  {0, (36), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
			  {0, (48), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			  D3DDECL_END()
			};
			_Device->CreateVertexDeclaration(decl, &VertexDeclaration);
			return VertexDeclaration;
			//_Device->SetVertexDeclaration(VertexDeclaration);
		}
	};

	struct MtrlInfo
	{
		enum class Illumination :uint8_t
		{

		};
		D3DXCOLOR Ambient{ 0,0,0,0.f };
		D3DXCOLOR Diffuse = { 0,0,0,1.f };
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
		MtrlInfo MaterialInfo{};

		LPDIRECT3DVERTEXDECLARATION9 Decl{ nullptr };
		IDirect3DVertexBuffer9* VtxBuf{ nullptr };
		IDirect3DTexture9* Texture{ nullptr };
		IDirect3DTexture9* TextureNormal{ nullptr };
		IDirect3DTexture9* TextureSpecular{ nullptr };
	};
protected:
	Shader::Info _ShaderInfo;
	std::shared_ptr<std::vector<Info>> _InfosPtr;
	std::shared_ptr<std::vector<PlaneInfo>> _PolygonPlane;
	mat MapWorld;
	DWORD MapAmbient = 0x00202020;
};

#define __MapBase_H__
#endif