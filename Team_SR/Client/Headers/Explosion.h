#pragma once
#ifndef __EXPLOSION_H__

#include "Bullet.h"
USING(Engine)
class CExplosion final : public CBullet
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CExplosion() = default;
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
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동

private:
	virtual HRESULT AddComponents() override;

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	bool bRelayFlag;	// 전달 여부
};

#define __EXPLOSION_H__
#endif