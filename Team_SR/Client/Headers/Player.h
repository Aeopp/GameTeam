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
	virtual void Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo);
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
		ShotGun,
		Akimbo,
		Magnum,
		Staff,
	};
public:
	FORCEINLINE CPlayer::EWeaponState GetWeaponState()const& { return _CurrentWeaponState; };
	CCollisionComponent* _CollisionComp = nullptr;
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

	bool bUpgrade = false; 
	bool bKeyBlue = false;
	bool bKeyYellow = false;
	bool bKeyRed = false;
	int32_t Ammo = 100l;
	float HP = 100.f;
	float MP = 100.f;
private:
	class CNormalUVVertexBuffer* _VertexBuffer{ nullptr };
	AnimationTextures _AnimationTextures;
	vec3 PrevLocation{ 0 , 0 , 0 };
	float T = 0.0f;
	EWeaponState _CurrentWeaponState = EWeaponState::Dagger;
	bool bStaffLoop = false;
private:
	const float WeaponAnimDelta = 0.07f;
	void ShotGunShot();
	void ShotGunReload();
	void DaggerStab();
	void DaggerThrow();
	void AkimboFire();
	void MagnumFire();
	void StaffFire();
	void StaffCharge();
	void StaffRelease();
	void StaffLoop();

	void PushLightFromName(const std::wstring& LightName)&;

	std::map<std::wstring, float> LightingDurationTable
	{
		{L"ShotGunShot",0.f}, 
		{L"AkimboFire",0.f},
		{L"MagnumFire",0.f},
		{L"StaffFire",0.f},
		{L"StaffCharge",0.f},
		{L"StaffRelease",0.f}
	};
	void PlayStepSound();
private:
	int	m_iStepIndex = 0;
};

#define __PLAYER_H__
#endif