#pragma once
#ifndef __MONSTER_H__

#include "GameObject.h"
#include "DXWrapper.h"
#include "Vertexs.h"
#include "JumpPointSearch.h"

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
	virtual void MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo)override;
	virtual void ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo);
	void FlashHit()&;
	virtual void FreezeHit()=0;
	bool Attack(const Sphere _Sphere, const float Attack) &;
	bool Attack(const Ray _Ray, const float Attack) &;
	void MeleeAttack();
	FORCEINLINE bool IsDead()const& { 
		return   ( (m_byMonsterFlag & static_cast<BYTE>(CMonster::MonsterFlag::Dead )) == (BYTE)MonsterFlag::Dead); };
protected:
	bool Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동 - 프레임 카운트가 End에 도달하면 true, 아니면 false
	bool PlayerAwareness();					// 플레이어 인식 - 인식하면 true, 인식하지 못하면 false
	bool PlayerBeNear();					// 플레이어가 가까이 근접해 있으면 true, 아니면 false
	void CollisionMovement(float fDeltaTime);	// 충돌 이동
	void CreateBlood();
	void CreateFloorBlood();
	void PathFinding(vec3 _vDepa, vec3 _vDest);	// 길찾기
	void BloodParticle();
	void DeadProcess();
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	// 플래그 값들
	enum class MonsterFlag {
		HPLock				= 1,			// HP 락 - 피해를 입지 않음, HP 깍이는 함수에서 예외처리로 용으로 쓸 것
		Dead				= 1 << 1,		// 죽음
		Shoot				= 1 << 2,		// 총쏨
		TextureChangeLock	= 1 << 3,		// 텍스처 체인지 락 - 텍스처 교체 가능 여부
		PlayerTracking		= 1 << 4		// 플레이어 추적
		// ... 이 밑으로 3개 예약 가능!!
	};
protected:
	float _CurrentDeltaTime = 0.0f;
	float FreezeHitDamage = 12.f;
	float LightHitTime = 0.0f;
	float FreezeDeadProcessTime = 0.0f;
	float FreezeBloodParticleTime = 0.0f;
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
	list<vec3> m_listMovePos;				// 이동 좌표 리스트
	JumpPointSearch* m_pJumpPointSearch;	// 길찾기
	bool m_bFrameLoopCheck;					// 프레임 루프
	bool m_bNoLoop;							// 루프를 돌지 않음
	BYTE m_byMonsterFlag;					// 플래그 변수 enum MonsterFlag 참조
	std::vector<size_t> GibTable;
	const float FloorBloodCoolTime = 1.f;
	float FloorBloodCurrentCoolTime = FloorBloodCoolTime;
public:
/// 조명 하이라이팅이 강해짐.
	float Shine = 20.f;
	//                   렌더링 컴포넌트
	class CNormalUVVertexBuffer * _VertexBuffer{ nullptr };
	      // Degree
	float RotationXZPlane = 0.0f;
};

#define  __MONSTER_H__
#endif
