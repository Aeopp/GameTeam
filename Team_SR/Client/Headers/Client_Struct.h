#pragma once

#ifndef __CLIENT_STRUCT_H__

// 몬스터 스텟
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

// 몬스터 생성시 기본 전달 인자
struct MonsterBasicArgument {
	_uint uiSize;				// 구조체 사이즈
	class CPlayer* pPlayer;		// 플레이어 포인터
	vec3 vPosition;				// 위치 정보
};

struct BulletBasicArgument {
	_uint uiSize;				// 구조체 사이즈
	vec3 vPosition;				// 위치 정보
	vec3 vDir;					// 방향
};

#endif // !__CLIENT_STRUCT_H__
