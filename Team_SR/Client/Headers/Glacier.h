#pragma once
#ifndef __GLACIER_H__

#include "Monster.h"
USING(Engine)
class CGlacier final : public CMonster
{
private:
	explicit CGlacier(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGlacier() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	virtual HRESULT AddComponents() override;
public:
	static CGlacier* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#define __GLACIER_H__
#endif