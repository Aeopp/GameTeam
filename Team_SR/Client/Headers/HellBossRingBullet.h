#pragma once
#ifndef __HELLBOSSRINGBULLET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossRingBullet final : public CBullet
{
private:
	explicit CHellBossRingBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossRingBullet() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동

private:
	virtual HRESULT AddComponents() override;

public:
	static CHellBossRingBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
};

#define __HELLBOSSRINGBULLET_H__
#endif