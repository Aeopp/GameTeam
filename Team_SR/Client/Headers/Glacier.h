#pragma once
#ifndef __GLACIER_H__

#include "Monster.h"
USING(Engine)
class CGlacier final : public CMonster
{
private:
	explicit CGlacier(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGlacier() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	// 2020.12.17 9:57 KMJ
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;	// 몬스터가 피해를 받음
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;
	virtual void ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)override;
private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();
	
private:
	void Update_AI(float fDeltaTime);

	////////////////////AWARENESS//////////////////
	void AI_NoAwareness();
	void AI_FirstPhase();
	void AI_SecondPhase();
	void AI_ThirdPhase();
	void AI_DeadPhase();
	////////////////////ACTION/////////////////////
	bool Action_Move(float fDeltaTime);
	bool Action_Idle(float fDeltaTime);
	bool Action_Shoot(float fDeltaTime);
	bool Action_Hurt(float fDeltaTime);
	bool Action_Death(float fDeltaTime);
	///////////////////////////////////////////////
	void CreateBullet();
	void CreateParticle();
private:
	enum class AWARENESS { No, Yes, End };
	enum class PHASE { HP_High , HP_Half, HP_Low, HP_ZERO ,End };

	using AIFunc = void(CGlacier::*)(void);
	using ACTFunc = bool(CGlacier::*)(float);

public:
	static CGlacier* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float FreezeGlacierParticleTime = 0.0f;
	float m_fCountDown = 0.f;
	bool  m_bDead = false;
	wstring m_wstrBase;
	wstring m_ComTexture;
	////////////////////////////////////////////////////
	AWARENESS	m_eAwareness = AWARENESS::End;
	PHASE		m_ePhase = PHASE::End;
	AIFunc		m_fpGlacierAI[(_uint)AWARENESS::End][(_uint)PHASE::End];
	ACTFunc		m_fpAction;

	// CMonster을(를) 통해 상속됨
	virtual void FreezeHit() override;
};

#define __GLACIER_H__
#endif