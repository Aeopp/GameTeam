#pragma once
#ifndef GlacierParticle_h__
#define GlacierParticle_h__

#include "Effect.h"
class CGlacierParticle final : public CEffect
{
private:
	explicit CGlacierParticle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGlacierParticle() = default;

public:
	// CEffect을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	virtual HRESULT AddComponents() override;
public:
	static CGlacierParticle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#endif // GlacierParticle_h__