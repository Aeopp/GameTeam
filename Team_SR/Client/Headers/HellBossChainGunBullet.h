#pragma once
#ifndef __HELLBOSSCHAINGUNBULLET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossChainGunBullet final : public CBullet
{
private:
	explicit CHellBossChainGunBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossChainGunBullet() = default;
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
	static CHellBossChainGunBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matRot;
};

#define __HELLBOSSCHAINGUNBULLET_H__
#endif