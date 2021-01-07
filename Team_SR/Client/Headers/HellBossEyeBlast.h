#pragma once
#ifndef __HELLBOSSEYEBLAST_H__

#include "Bullet.h"
USING(Engine)
class CHellBossEyeBlast final : public CBullet
{
private:
	explicit CHellBossEyeBlast(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossEyeBlast() = default;
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
	static CHellBossEyeBlast* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matRot;
};

#define __HELLBOSSEYEBLAST_H__
#endif