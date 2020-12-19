#pragma once
#ifndef __MONSTER_H__

#include "GameObject.h"
#include "DXWrapper.h"
#include "Vertexs.h"



USING(Engine)
class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
protected:
	virtual HRESULT AddComponents();
private:
	HRESULT IsBillboarding();
public:
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;	// 몬스터가 피해를 받음
protected:
	bool Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동 - 프레임 카운트가 End에 도달하면 true, 아니면 false
	bool PlayerAwareness();					// 플레이어 인식 - 인식하면 true, 인식하지 못하면 false
	bool PlayerBeNear();					// 플레이어가 가까이 근접해 있으면 true, 아니면 false
	void CollisionMovement(float fDeltaTime);	// 충돌 이동
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
protected:
	// 플래그 값들
	enum class MonsterFlag {
		HPLock	= 1,			// HP 락 - 피해를 입지 않음, HP 깍이는 함수에서 예외처리로 용으로 쓸 것
		Dead	= 1 << 1,		// 죽음
		Shoot	= 1 << 2		// 총쏨
		// ... 이 밑으로 5개 예약 가능!!
	};
protected:
	float m_fFrameCnt;						// 프레임 번호
	float m_fStartFrame;					// 프레임 시작
	float m_fEndFrame;						// 프레임 끝
	float m_fFrameSpeed;					// 프레임 속도
	float m_fCrossValue;					// 충돌 힘
	vec3 m_vCollisionDir;					// 충돌 방향
	vec3 m_vAim;							// 목표
	CGameObject* m_pPlayer;					// 플레이어 포인터
	MonsterStatus m_stOriginStatus;			// 몬스터 원본 스텟
	MonsterStatus m_stStatus;				// 몬스터 스텟
	wstring m_wstrTextureKey;				// 텍스처 키
	map<wstring, CTexture*> m_mapTexture;	// 텍스처 맵
	bool m_bFrameLoopCheck;					// 프레임 루프
	BYTE m_byMonsterFlag;					// 플래그 변수 enum MonsterFlag 참조

public:
/// 조명 하이라이팅이 강해짐.
	float Shine = 20.f;
	//                   렌더링 컴포넌트
	class CNormalUVVertexBuffer * _VertexBuffer{ nullptr };
/// 
};

#define  __MONSTER_H__
#endif
