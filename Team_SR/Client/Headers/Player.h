#pragma once
#ifndef __PLAYER_H__

#include "GameObject.h"
#include "CollisionComponent.h"

USING(Engine)
class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayer() = default;
public:
	using Super = CGameObject;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

	void MoveForward(const float DeltaTime)&;
	void MoveRight(const float DeltaTime)&;
private:
	HRESULT AddStaticComponents()override;
public:
	CCollisionComponent* _CollisionComp = nullptr;
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#define __PLAYER_H__
#endif