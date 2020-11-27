#pragma once
#ifndef __TERRAIN_H__

#include "GameObject.h"

USING(Engine)
class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& other);
	virtual ~CTerrain() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	HRESULT AddComponent();

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;	
	class CTransform* m_pTransformCom = nullptr;
	class CTexture*	m_pTextureCom = nullptr;
};

#define __TERRAIN_H__
#endif