#pragma once
#ifndef __PLAYER_H__

#include "GameObject.h"
#include "CollisionComponent.h"
#include "DXWrapper.h"

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
	virtual void MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo)override;
	void MoveForward(const float DeltaTime)&;
	void MoveRight(const float DeltaTime)&;
	void MouseRight()&;
	void MouseRightPressing()&;
	void MouseRightUp()&;
	void MouseLeft()&;
	void MouseLeftPressing()&;
	void RButtonEvent()&;
	void _1ButtonEvent()&;
	void _2ButtonEvent()&;
	void _3ButtonEvent()&;
	void _4ButtonEvent()&;
	void _5ButtonEvent()&;
private:
	HRESULT AddStaticComponents()override;
	enum class EWeaponState : uint8_t
	{
		Dagger,
		Harvester,
		Akimbo,
		Magnum,
		Staff,
	};
public:
	CCollisionComponent* _CollisionComp = nullptr;
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	std::shared_ptr<std::vector<SubSetInfo>> _Quad;
	AnimationTextures _AnimationTextures;
	EWeaponState _CurrentWeaponState = EWeaponState::Dagger;
	bool bStaffLoop = false;
private:
	const float WeaponAnimDelta = 0.07f;
	void HarvesterFire();
	void HarvesterReload();
	void DaggerStab();
	void DaggerThrow();
	void AkimboFire();
	void MagnumFire();
	void StaffFire();
	void StaffCharge();
	void StaffRelease();
	void StaffLoop();
};

#define __PLAYER_H__
#endif