#pragma once
#ifndef __HELLBOSSEYELASER_H__

#include "Bullet.h"
USING(Engine)
class CHellBossEyeLaser final : public CBullet
{
private:
	explicit CHellBossEyeLaser(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossEyeLaser() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;

private:
	virtual HRESULT AddComponents() override;

public:
	static CHellBossEyeLaser* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matRot;
};

#define __HELLBOSSEYELASER_H__
#endif