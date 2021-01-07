#pragma once
#ifndef __GHOUL_H__

/*
땅속에 숨어있다가 플레이어가 접근하면 땅을 파고 나옴
행동이 느림
땅속에 숨어있을 때는 데미지를 받으면 안됨
*/

#include "Monster.h"
USING(Engine)
class CGhoul final : public CMonster
{
private:
	explicit CGhoul(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGhoul() = default;
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
	virtual void MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo) override;

private:
	void Update_AI(float fDeltaTime);		// 업데이트 AI

	void AI_NoAwareness();					// 플레이어를 인식하지 못함
	void AI_NormalPattern();				// 일반 패턴

	bool Action_Idle(float fDeltaTime);		// 행동 대기
	bool Action_DigOut(float fDeltaTime);	// 땅파고 나옴
	bool Action_Move(float fDeltaTime);		// 이동
	bool Action_Melee(float fDeltaTime);	// 근접 공격
	bool Action_Hit(float fDeltaTime);		// 공격받아서 경직
	bool Action_Dead(float fDeltaTime);		// 죽음

public:
	static CGhoul* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class AWARENESS { No, Yes, End };			// 인식 - 플레이어를 인식했는가

	using AIFunc = void(CGhoul::*)(void);		// AI 함수
	using ACTFunc = bool(CGhoul::*)(float);		// 몬스터 행동 함수

private:
	float m_fCountdown;			// 행동 카운트
	float m_fNextAtkWait;		// 다음 공격 대기
	float m_fPlayerTrackCount;	// 플레이어 추적 카운트
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 true, 진행시 false
	AWARENESS m_eAwareness;		// 인식
	AIFunc m_fpMonsterAI[(int)AWARENESS::End];	// AI 함수 배열
	bool isHide;				// 몬스터가 땅속에 숨어있는지 확인용
};

#define __HELLHOUND_H__
#endif