#pragma once
#ifndef __BATGREY_H__

#include "Monster.h"
USING(Engine)
class CBatGrey final : public CMonster
{
private:
	explicit CBatGrey(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBatGrey() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	virtual HRESULT AddComponents() override;
	
private:
	void Update_AI(float fDeltaTime);		// 업데이트 AI
	HRESULT Set_Texture();					// 장치에 텍스처 셋

	void AI_NoAwareness();					// 플레이어를 인식하지 못함
	void AI_ActiveOffense();				// 적극적으로 공격
	void AI_PassiveOffense();				// 소극적으로 공격

	bool Action_Idle(float fDeltaTime);		// 행동 대기
	bool Action_Shoot(float fDeltaTime);	// 원거리 공격

public:
	static CBatGrey* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class AWARENESS { No, Yes, End };			// 인식 - 플레이어를 인식했는가
	enum class PHASE { HP_Full, HP_Half, End };		// 페이즈 - 현제 체력

	using AIFunc = void(CBatGrey::*)(void);		// AI 함수
	using ACTFunc = bool(CBatGrey::*)(float);	// 몬스터 행동 함수

private:
	float m_fCountdown;			// 카운트다운
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 true, 진행시 false
	AWARENESS m_eAwareness;		// 인식
	PHASE m_ePhase;				// 페이즈
	AIFunc m_fpMonsterAI[(int)AWARENESS::End][(int)PHASE::End];	// AI 함수 배열
};

#define __GLACIER_H__
#endif