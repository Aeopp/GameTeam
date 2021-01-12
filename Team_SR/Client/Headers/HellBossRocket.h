#pragma once
#ifndef __HELLBOSSROCKET_H__

#include "Bullet.h"
USING(Engine)
class CHellBossRocket final : public CBullet
{
private:
	explicit CHellBossRocket(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossRocket() = default;
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
	static CHellBossRocket* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	mat m_matHorizontalRot;		// 가로 회전
	mat m_matVerticalRot;		// 세로 회전
	mat m_matBackRot;			// 뒤 회전
	CTexture* m_pHorizontalTexture;	// 가로 텍스처
	CTexture* m_pVerticalTexture;	// 세로 텍스처
	CTexture* m_pBackTexture;		// 뒤 텍스처
	TRANSFORM_DESC m_pTransformVertical;	// 세로 위치
	TRANSFORM_DESC m_pTransformBack;		// 뒤 위치
};

#define __HELLBOSSROCKET_H__
#endif