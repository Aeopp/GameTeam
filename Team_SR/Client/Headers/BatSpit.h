#pragma once
#ifndef __BATSPIT_H__

#include "Bullet.h"
USING(Engine)
class CBatSpit final : public CBullet
{
private:
	explicit CBatSpit(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBatSpit() = default;
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
	static CBatSpit* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
};

#define __BATSPIT_H__
#endif