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
	
	void Update_AI(float fDeltaTime);		// 업데이트 AI
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	virtual HRESULT Set_Texture();			// 장치에 텍스처 셋

	void AI_NoPerception();					// 플레이어를 인식하지 못함
	void AI_ActiveOffense();				// 적극적으로 공격
	void AI_PassiveOffense();				// 소극적으로 공격

	BOOL Action_Idle(float fDeltaTime);		// 행동 대기

public:
	static CBatGrey* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class PERCEPTION { No, Yes, End };			// 인지력 - 플레이어를 인식했는가
	enum class PHASE { HP_Full, HP_Half, End };		// 페이즈 - 현제 체력

	using AIFunc = void(CBatGrey::*)(void);		// AI 함수
	using ACTFunc = BOOL(CBatGrey::*)(float);	// 몬스터 행동 함수
private:
	float m_fFrameCnt;						// 프레임 번호
	float m_fStartFrame;					// 프레임 시작
	float m_fEndFrame;						// 프레임 끝
	wstring m_wstrTextureKey;				// 텍스처 키
	map<wstring, CTexture*> m_mapTexture;	// 텍스처 맵
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 TRUE, 진행시 FALSE
	PERCEPTION m_ePerception;	// 인지
	PHASE m_ePhase;				// 페이즈
	AIFunc m_fpMonsterAI[(int)PERCEPTION::End][(int)PHASE::End];	// AI 함수 배열
};

#define __GLACIER_H__
#endif