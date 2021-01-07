#pragma once
#ifndef __HANGMAN_H__

/*
근접 공격과 원거리 폭탄 던지기 공격을함
큰 피해를 받아 손상되면 이동속도가 약간 빨라지고 근접 공격만 함
*/

#include "Monster.h"
USING(Engine)
class CHangman final : public CMonster
{
private:
	explicit CHangman(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHangman() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	virtual HRESULT AddComponents() override;
	
public:
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;	// 몬스터가 피해를 받음
	virtual void ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)override;

private:
	void Update_AI(float fDeltaTime);		// 업데이트 AI

	void AI_NoAwareness();					// 플레이어를 인식하지 못함
	void AI_NormalPattern();				// 일반 패턴
	void AI_DamagedPattern();				// 손상된 패턴

	bool Action_Idle(float fDeltaTime);		// 행동 대기
	bool Action_Move(float fDeltaTime);		// 이동
	bool Action_Shoot(float fDeltaTime);	// 원거리 공격
	bool Action_Melee(float fDeltaTime);	// 근접 공격
	bool Action_Hit(float fDeltaTime);		// 공격받아서 경직
	bool Action_Dead(float fDeltaTime);		// 죽음
	bool Action_Damage(float fDeltaTime);	// 손상

public:
	static CHangman* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class AWARENESS { No, Yes, End };			// 인식 - 플레이어를 인식했는가
	enum class PHASE { HP_Full, HP_Half, End };		// 페이즈 - 현제 체력

	using AIFunc = void(CHangman::*)(void);		// AI 함수
	using ACTFunc = bool(CHangman::*)(float);	// 몬스터 행동 함수

private:
	float m_fCountdown;			// 행동 카운트
	float m_fNextAtkWait;		// 다음 공격 대기
	float m_fPlayerTrackCount;	// 플레이어 추적 카운트
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 true, 진행시 false
	AWARENESS m_eAwareness;		// 인식
	PHASE m_ePhase;				// 페이즈
	AIFunc m_fpMonsterAI[(int)AWARENESS::End][(int)PHASE::End];	// AI 함수 배열
	bool isDamaged;
	// CMonster을(를) 통해 상속됨
	virtual void FreezeHit() override;
	// 텍스처 손상 전환용
};

#define __HANGMAN_H__
#endif