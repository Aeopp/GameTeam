#pragma once
#ifndef Blood_h__
#define Blood_h__


#include "Effect.h"
class CBlood final : public CEffect
{
private:
	explicit CBlood(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBlood() = default;

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
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CBlood* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
};


#endif // Blood_h__