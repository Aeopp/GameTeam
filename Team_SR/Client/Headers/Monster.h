#pragma once
#ifndef __MONSTER_H__

#include "GameObject.h"
USING(Engine)
class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
protected:
	virtual HRESULT AddComponents();
public:
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#define  __MONSTER_H__
#endif
