#pragma once
#ifndef __HELLBOSS_H__

/*
리틀 데몬
지옥을 통치하는 타락한 신의 제1형태

기출 크리처 중에서 가장 거대한 체구를 자랑하는 이 악마는, 자신의 왕궁에 침입한 워록(당신)과 결판을 내기 위해, 천천히 확실한 발걸음으로 전진해 온다.
공격 방법은 오른손으로부터 발사되는 링 모양의 광선과 외눈으로부터 조사되는 강력 무비한 레이저(모두 폭풍 효과 있음)다.모두 워록이 사정권에 들어가면,
무시무시한 속도로 연사해 오기 때문에, 광대한 에리어를 이용해 히트&어웨이를 반복해 가자.


터보 사탄
지옥을 통치하는 타락한 신의 제2형태

양팔에 체인건을 장착한 터보 사탄은 매우 장거리에서 워록(당신)을 향해 정확한 사격을 한다.
광활한 지역을 돌아다니기만 하면 총알을 피하기가 쉽지만 터보 사탄의 걸음은 빨라 움직이면서가 아니면 순식간에 따라잡을 수 없을 것이다.
또 터보 사탄은 일정량의 피해를 입히면 주 무기를 로켓추진총유기로 전환한다.
체인건만큼이나 사격이 정확하기 때문에 타격이 큰 로켓의 직격을 받지 않도록 조심하자.


카코 데빌
지옥을 통치하는 타락한 신의 제3형태

악몽의 구현 같은 모습을 한 이 악마는 그 세 눈동자에서 회피할 수 없는 레이저와 충격파를 발사하며 워록(당신)을 묻으려 한다.
또한 생명을 창조하는 능력을 가진 카코 데빌은 모든 에피소드에 등장한 크리처들을 무한 소환하기 때문에
강력한 무기나 철자를 이용하여 신속하게 공격을 가해 나가는 것이 최선의 방법이라고 할 수 있다.


몰락한 군주
지옥을 통치하는 타락한 신의 최종 형태
에피소드 4의 보스로 본작의 최종 보스.

꿈틀꿈틀 부정형의 끔찍한 모습을 드러내게 된 이 신은 가진 능력을 모두 행사해 워록(당신)을 매장시키려 한다.
공격 방법은 카코·데빌과 같이 크리쳐의 무한 소환이지만, 느릿느릿 촉수로 워록을 밟을 수 있도록 종횡 무진에 출몰시키기 때문에, 맥빠지지 않는 전투를 강요당하게 된다.
이제까지의 형태와 달리 정해진 위치에서는 움직일 수 없기 때문에 신출귀몰한 촉수와 크리처들을 피하고 지역의 보급물자를 주워 모으면서 이 타락한 신과 결말을 짓도록 하자.
*/

#include "Monster.h"
USING(Engine)
class CHellBoss final : public CMonster
{
private:
	explicit CHellBoss(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBoss() = default;
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

private:
	void Update_AI(float fDeltaTime);		// 업데이트 AI

	void AI_LittleDemonPattern();			// 리틀 데몬 패턴
	void AI_TurboSatanPattern();			// 터보 사탄 패턴
	void AI_InjuredTurboSatanPattern();		// 부상당한 터보 사탄 패턴
	void AI_CacoDevilPattern();				// 카코 데빌 패턴
	void AI_FallenLordPattern();			// 몰락한 군주 패턴

	bool Action_Idle(float fDeltaTime);		// 행동 대기
	bool Action_Move(float fDeltaTime);		// 이동
	bool Action_Morph(float fDeltaTime);	// 변신
	bool Action_LastMorph(float fDeltaTime);// 최종 페이즈 변신
	bool Action_LittleDemon_EyeBlast(float fDeltaTime);		// 리틀 데몬 눈깔 빔
	bool Action_LittleDemon_Shoot(float fDeltaTime);		// 리틀 데몬 원거리 공격
	bool Action_TurboSatan_ShootStart(float fDeltaTime);	// 터보 사탄 원거리 공격 시작
	bool Action_TurboSatan_ShootSpin(float fDeltaTime);		// 터보 사탄 원거리 공격 회전
	bool Action_TurboSatan_ShootFire(float fDeltaTime);		// 터보 사탄 원거리 공격 발싸
	bool Action_TurboSatan_ShootEnd(float fDeltaTime);		// 터보 사탄 원거리 공격 끝
	bool Action_InjuredTurboSatan_Shoot(float fDeltaTime);	// 부상당한 리틀 데몬 원거리 공격
	bool Action_CacoDevil_EyeLasers(float fDeltaTime);		// 카코 데빌 눈깔 레이저
	bool Action_CacoDevil_Nova(float fDeltaTime);			// 카코 데빌 근접 충격파
	bool Action_CacoDevil_MonsterSpawn(float fDeltaTime);	// 카코 데빌 몬스터 소환
	bool Action_FallenLord_TentacleAttack(float fDeltaTime);// 몰락한 군주 촉수 공격
	bool Action_FallenLord_MonsterSpawn(float fDeltaTime);	// 몰락한 군주 몬스터 소환
	bool Action_Dead(float fDeltaTime);		// 죽음

public:
	static CHellBoss* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	enum class PHASE { LittleDemon, TurboSatan, InjuredTurboSatan, CacoDevil, FallenLord, End };		// 페이즈

	using AIFunc = void(CHellBoss::*)(void);		// AI 함수
	using ACTFunc = bool(CHellBoss::*)(float);		// 몬스터 행동 함수

private:
	float m_fCountdown;			// 행동 카운트
	float m_fNextAtkWait;		// 다음 공격 대기
	float m_fPlayerTrackCount;	// 플레이어 추적 카운트
	int m_iRepeatCount;			// 반복 횟수
	ACTFunc m_fpAction;			// 현재 몬스터 행동 함수 - 행동 완료시 true, 진행시 false
	PHASE m_ePhase;				// 페이즈
	AIFunc m_fpMonsterAI[(int)PHASE::End];	// AI 함수 배열
};

#define __HELLHOUND_H__
#endif