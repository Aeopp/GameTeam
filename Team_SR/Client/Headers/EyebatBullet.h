#pragma once
#ifndef EyebatBullet_h__
#define EyebatBullet_h__

#include "Bullet.h"
USING(Engine)
class CEyebatBullet final : public CBullet
{
private:
	explicit CEyebatBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEyebatBullet() = default;
public:
	// CBullet을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;
private:
	virtual HRESULT AddComponents() override;
	void Movement(float fDeltaTime);
	void CreateFire();
public:
	static CEyebatBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	_vector m_vLook;
	float m_fAngle = 0.f;
	float m_fStartY = 0.f;
	float m_fJumpPower = 0.f;
	float m_fJumpTime = 0.f;
	bool  m_bTest = false;
};

#endif // EyebatBullet_h__