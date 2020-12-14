#pragma once

#ifndef __CLIENT_STRUCT_H__

// 몬스터 스텟
struct MonsterStatus {
	float fHP;		// 체력
	float fATK;		// 공격력
	float fDEF;		// 방어력
	float fSpeed;	// 이동 속도
	float fDetectionDistance;	// 감지 거리
};

// 몬스터 생성시 기본 전달 인자
struct MonsterBasicArgument {
	_uint uiSize;				// 구조체 사이즈
	class CPlayer* pPlayer;		// 플레이어 포인터
	vec3 vPosition;				// 위치 정보
};

#endif // !__CLIENT_STRUCT_H__
