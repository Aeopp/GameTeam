#pragma once
#ifndef Shark_h__
#define Shark_h__

#include "Monster.h"
USING(Engine)
class CShark final :	public CMonster
{
private:
	explicit CShark(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShark() = default;

public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;

private:
	void Update_AI(float fDeltaTime);

	////////////////////PHASE//////////////////
	void AI_FirstPhase();
	void AI_SecondPhase();
	void AI_ThridPhase();
	void AI_DeadPhase();
	////////////////////ACTION/////////////////////
	bool Action_Move(float fDeltaTime);
	bool Action_Idle(float fDeltaTime);
	bool Action_Shoot(float fDeltaTime);
	bool Action_Dash(float fDeltaTime);
	bool Action_Warigari(float fDeltaTime);
	bool Action_HighJump(float fDeltaTime);
	bool Action_MeleeAttack(float fDeltaTime);
	bool Action_Death(float fDeltaTime);
	/////////////////////////
	void CreateBullet(float fDeltaTime);
	void UpdateDir(float fDeltaTime);
private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();

private:
	enum class PHASE { HP_High, HP_Middle, HP_Low, HP_ZERO, End };

	using AIFunc = void(CShark::*)(void);
	using ACTFunc = bool(CShark::*)(float);
public:
	static CShark* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;


private:
	wstring		m_wstrBase;
	float		m_fCountDown = 0.f;
	float		m_fChagneDirTime = 0.f;
	float		m_fBulletCoolDown = 0.f;
	int			m_iBulletCount = 0;
	int			m_iDir = 0.f;
	float		m_fNextAtkWait = 0.f;
	PHASE		m_ePhase = PHASE::End;
	AIFunc		m_fpSharkAI[(_uint)PHASE::End];
	ACTFunc		m_fpAction;
};

#endif // Shark_h__