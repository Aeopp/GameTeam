#pragma once
#include "stdafx.h"

#ifndef __CLIENT_STRUCT_H__

//Player Info
typedef struct tagPlayerInfo
{
	//체력
	_int iMaxHP;
	_int iMinHP;

	//마나
	_int iMaxMana;
	_int iMinMana;
}PLAYER_INFO;

typedef struct tagWeaponInfo
{
	//총알 수
	int32_t iMaxAmmo;
	int32_t iMinAmmo;
}WEAPON_INFO;

//몬스터 스텟
struct MonsterStatus {
	float fHP;		// 체력
	float fATK;		// 공격력
	float fDEF;		// 방어력
	float fSpeed;	// 이동 속도
	float fMeleeRange;		// 근접 공격 범위
	float fDetectionRange;	// 감지 범위
};

// 총알 스텟
struct BulletStatus {
	DWORD dwPiercing;		// 관통력 - 한발의 총알로 관통되어 맞출 수 있는 정도 ( 0 이면 관통하지않고 사라짐 1 이상이면 충돌마다 1씩 차감 후 사라짐)
	float fRange;			// 사거리 - 총알이 무한정 앞으로 나갈순 없음
	float fATK;				// 공격력
	float fSpeed;			// 총알 속도
	float fImpact;			// 충격력 - 총알에 맞은 적을 뒤로 물러나게 하는 정도
};

// 아이템 정보
struct ItemInfo {
	ITEM etype;				// 아이템 종류
	int iAmount;			// 해당 아이템 효과의 양? tag = HP, iAmout = 50 이면 HP 50회복 이런식?
};

// 장식 정보
struct DecoratorInfo {
	DECO eType;				// 장식 종류
	float fHP;				// 해당 장식이 파괴되는 물체면 HP값 셋팅
};

// 장식 다음 프레임 정보 구조체
struct DecoNextFrameInfo {
	float fStartFrame;		// 프레임 시작
	float fEndFrame;		// 프레임 끝
	float fTriggerHP;		// 다음 프레임 구간으로 넘어갈 체력 : 값이 -1.f 이면 마지막 프레임
};

// 몬스터 생성시 기본 전달 인자
struct MonsterBasicArgument {
	MonsterBasicArgument()
		: uiSize(sizeof(MonsterBasicArgument)), pPlayer(nullptr), vPosition{ 0.f,0.f,0.f }, bDeleteFlag(false)
	{}
	_uint uiSize;				// 구조체 사이즈
	class CPlayer* pPlayer;		// 플레이어 포인터
	vec3 vPosition;				// 위치 정보
	bool bDeleteFlag;			// delete 플래그 - 스택에서 만들었으면 false, 힙이면 true
};

struct BulletBasicArgument {
	BulletBasicArgument()
		: uiSize(sizeof(BulletBasicArgument)), vPosition{ 0.f,0.f,0.f }, vDir{ 0.f,0.f,0.f }, uiCountRelay(0)
	{}
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	vec3 vDir;					// 방향
	_uint uiCountRelay;			// 특정 총알 전용 변수 - 전달 카운트
};

// 아이템 생성시 기본 전달 인자
struct ItemBasicArgument {
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	ITEM etype;					// 아이템 종류
	bool bDeleteFlag;			// delete 플래그 - 스택에서 만들었으면 false, 힙이면 true
};

// 이펙트 생성시 기본 전달 인자
struct EffectBasicArgument {
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	EFFECT eType;				// 이펙트 종류
};

// 장식 생성시 기본 전달 인자
struct DecoratorBasicArgument {
	DecoratorBasicArgument()
		:uiSize(sizeof(DecoratorBasicArgument)), vPosition{0.f,0.f,0.f}, eType(DECO::End), bDeleteFlag(false)
	{}
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	DECO eType;					// 장식 종류
	bool bDeleteFlag;			// delete 플래그 - 스택에서 만들었으면 false, 힙이면 true
};

// 포탈 생성시 기본 전달 인자
struct WormholeArgument {
	WormholeArgument()
		:uiSize(sizeof(WormholeArgument)), vPosition{ 0.f,0.f,0.f }, eReplaceSceneID(ESceneID::MaxCount), bDeleteFlag(false)
	{}
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	ESceneID eReplaceSceneID;		// 교체할 씬 번호
	bool bDeleteFlag;			// delete 플래그 - 스택에서 만들었으면 false, 힙이면 true
};

//UI
typedef struct tagUIDesc
{
	tagUIDesc()
	{
		ZeroMemory(this, sizeof(tagUIDesc));
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);
		D3DXMatrixIdentity(&matWorld);
	}
	_vector	vUIPos;
	_vector	vUISize;
	_vector vCenter; //각 축당 -1 ~ 1

	_matrix matWorld;
	_matrix matView;
	_matrix matOrthographic;
}UI_DESC;

#endif // !__CLIENT_STRUCT_H__
