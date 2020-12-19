#pragma once
#ifndef Spider_h__
#define Spider_h__

#include "Monster.h"
USING(Engine)
class CSpider final : public CMonster
{
private:
	explicit CSpider(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpider() = default;

private:
	enum class DIR{LEFT,RIGHT,UP,DOWN,DIR_END};

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
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo);
private:
	void Update_AI(float fDeltaTime);
	void Z_Billboard();
	void ChagneSpeed(float fDeltaTime);
	void Jump(float fDeltaTime);
	////////////////////AWARENESS//////////////////
	void AI_NoAwareness();
	void AI_FirstPhase();
	//void AI_SecondPhase();
	void AI_DeadPhase();
	////////////////////ACTION/////////////////////
	bool Action_Move(float fDeltaTime);
	bool Action_Idle(float fDeltaTime);
	bool Action_Shoot(float fDeltaTime);
	bool Action_Death(float fDeltaTime);
	///////////////////////////////////////////////
	//void CreateBullet();


private:
	enum class AWARENESS { No, Yes, End };
	enum class PHASE { HP_High, HP_ZERO, End };

	using AIFunc = void(CSpider::*)(void);
	using ACTFunc = bool(CSpider::*)(float);

private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();

public:
	static CSpider* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float		m_fCountDown = 0.f;
	////////////////////////테스트////////////////////////////
	float		m_fTest = 0.f;
	float		m_fChangeSpeed = 0.f;
	float		m_fDashSpeed = 0.f;
	_vector		m_vLook;
	bool		m_bTest = false;
	float		m_fStartY = 0.f;
	float		m_fJumpPower = 0.f;
	float		m_fJumpTime = 0.f;
	////////////////////////////////////////////////////
	wstring		m_wstrBase;
	////////////////////////////////////////////////////
	AWARENESS	m_eAwareness = AWARENESS::End;
	PHASE		m_ePhase = PHASE::End;
	DIR			m_eDir = DIR::DIR_END;
	AIFunc		m_fpSpiderAI[(_uint)AWARENESS::End][(_uint)PHASE::End];
	ACTFunc		m_fpAction;
};

#endif // Spider_h__