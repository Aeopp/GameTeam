#pragma once
#ifndef __CMiniMap_H__

#include "Bullet.h"
USING(Engine)
class CMiniMap final : public CGameObject
{
private:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMiniMap() = default;
public:
	using Super = CGameObject;
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CMiniMap* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	vec3 MiniMapScale{ 1,1,1 };
	uint32_t MiniMapVertexCount;
	uint32_t MiniMapTriangleCount;
	uint16_t MiniMapVertexByteSize;
	std::shared_ptr<IDirect3DVertexBuffer9> MiniMapVertexBuf;
	std::shared_ptr<IDirect3DVertexDeclaration9> MiniMapVertexDecl;

	uint32_t PlayerMarkerVertexCount;
	uint32_t PlayerMarkerTriangleCount;
	uint16_t PlayerMarkerVertexByteSize;
	std::shared_ptr<IDirect3DTexture9> PlayerMarkerTexture;
	std::shared_ptr<IDirect3DVertexBuffer9> PlayerMarkerVertexBuf;
	std::shared_ptr<IDirect3DVertexDeclaration9> PlayerMarkerVertexDecl;
public:
	struct InitializeInfo 
	{
		std::vector<vec3> _Points;
		mat MapWorld;
	};
};

#define __CMiniMap_H__
#endif