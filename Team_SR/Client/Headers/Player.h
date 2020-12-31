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
	enum class EWeaponState : uint8_t
	{
		Dagger,
		ShotGun,
		Akimbo,
		Magnum,
		Staff,
		Dynamite,
		ElectricStaff,
		Flak,
	};
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
	void MouseLeftUp()&;
	void RButtonEvent()&;
	void _1ButtonEvent()&;
	void _2ButtonEvent()&;
	void _3ButtonEvent()&;
	void _4ButtonEvent()&;
	void _5ButtonEvent()&;
	void _6ButtonEvent()&;
	void _7ButtonEvent()&;
	void _8ButtonEvent()&;
	void SpellFreeze();
	void SpellLight();
private:
	HRESULT AddStaticComponents()override;
public:
	FORCEINLINE CPlayer::EWeaponState GetWeaponState()const& { return _CurrentWeaponState; };
	CCollisionComponent* _CollisionComp = nullptr;
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public: //변수
	bool bUpgrade = false; 
	bool bKeyBlue = false;
	bool bKeyYellow = false;
	bool bKeyRed = false;
public:
	//struct PlayerInfo
	//{
	//	const int32_t MaxAmmo = 100l;
	//	const float MaxHP = 100.0f;
	//	const float MaxMP = 100.f;
	//	int32_t Ammo = 0l;
	//	float HP = MaxHP;
	//	float MP = MaxMP;
	//};
public:
//	FORCEINLINE const PlayerInfo& GetPlayerInfo()const& { return _CurrentInfo; };

	WEAPON_INFO m_tWeaponInfo;
	//int32_t mMaxAmmo = 100l;
	//int32_t Ammo = 100l;

	PLAYER_INFO m_tPlayerInfo;
	//float mMaxHP = 100.f;
	//float HP = 100.f;
	//float mMaxMP = 100.f;
	//float MP = 100.f;

private:
	//PlayerInfo _CurrentInfo;
	class CNormalUVVertexBuffer* _VertexBuffer{ nullptr };
	AnimationTextures _AnimationTextures;
	AnimationTextures _WeaponEffectAnimTextures;
	vec3 PrevLocation{ 0 , 0 , 0 };
	float T = 0.0f;
	EWeaponState _CurrentWeaponState = EWeaponState::Dagger;
	bool bStaffLoop = false;
	float StaffChargeT = 0.0f;
	const float StaffAttack = 15.f;
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
	void DynamiteThrow();
	void FlakFire();
	void FlakReload();
	void ElectricStaffFire();
	void FreezeParticlePush()&;
	bool bWeaponEffectRender = false;
	void WeaponEffectOrthoRender(Effect::Info& _Effect);
	void PushLightFromName(const std::wstring& LightName , const float Duration)&;
	void ShieldStart()&;

	uint8_t CalcShieldStep()const&;
	std::map<std::wstring, float> LightingDurationTable
	{
		{L"ShotGunShot",0.f}, 
		{L"AkimboFire",0.f},
		{L"MagnumFire",0.f},
		{L"StaffFire",0.f},
		{L"StaffCharge",0.f},
		{L"StaffRelease",0.f} ,
		{L"ElectricStaffFire",0.f},
		{L"FlagFire",0.f} ,
		{L"SpellLight",0.f}
	};
	void PlayStepSound();
private:
	float RemainShield = 0.0f;
	float _DeltaTime = 0.0f;
	const vec3 CalcElectricStaffGizmo()const&;
	const float ElectricStaffFireRich = 13.f;
	int	m_iStepIndex = 0;
	float CurrentElectricStaffFireDamage = 1.0f;
	float ElectricStaffReinForceTimeRequired = 5.0f;
	const float ElectricStaffLightRadiusCoefft = 100.f;
	const float ElectricStaffLightDiffuseCoefft = 1.f / 10.f;
	const float SpellLightRadius = 500.0f;
	const float SpellLightHitRadius = 50.0f;
	const float SpellFreezeHitRadius = 50.0f;
	static constexpr float SpellLightDuration = 2.f;
	// 1초당 데미지로 정의된 테이블.
	std::vector<float> ElectricStaffDamageLimitTable
	{
		10.0f,
		20.0f,
		30.0f
	};
};

#define __PLAYER_H__
#endif