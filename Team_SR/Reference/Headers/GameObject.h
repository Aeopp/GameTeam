#pragma once
#ifndef __GAMEOBJECT_H__

#include "Base.h"
#include "Collision.h"
#include "CollisionComponent.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGameObject() = default;

public:
	class CComponent* GetComponent(const wstring & ComponentTag);
public:
	virtual HRESULT ReadyGameObjectPrototype() = 0;	/* 프로토타입 초기화 */
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0; /* 클론 초기화 */
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
	//            업데이트와 늦은 업데이트 이후 충돌 되면 호출. 
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo);
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo);
	virtual void ParticleHit(void* const _Particle, const Collision::Info & _CollisionInfo);
	void SetLocation(const vec3 Location);
	static const std::wstring Tag;
	class CTransform* GetTransform() { return m_pTransformCom; };
	// 2020.12.16 11:54 KMJ
	// 오브젝트 플래그 값을 가져옵니다
	BYTE GetOBjFlag() { return m_byObjFlag; }
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
	float CurrentAttack = 0.0f;
protected:
	HRESULT AddComponent(
		_int iSceneIndex,
		const wstring & PrototypeTag,
		const wstring & ComponentTag,
		class CComponent** ppComponent,
		void* pArg = nullptr);

	virtual HRESULT AddStaticComponents();
protected:
	LPDIRECT3DDEVICE9	m_pDevice;

	typedef unordered_map<wstring, class CComponent*> COMPONENTS;
	COMPONENTS	m_Components;

	//공통적으로 필요한 컴포넌트들
protected:
	class CTransform* m_pTransformCom = nullptr;
	class CManagement* m_pManagement = nullptr;
	CCollisionComponent* _CollisionComp = nullptr;
	bool bGravity = false;
public:
	// 2020.12.16 11:29 KMJ
	// 플래그 값들
	enum class ObjFlag {
		Remove = 1			// 삭제
		// ... 이 밑으로 7개 예약 가능!!
	};
protected:
	BYTE m_byObjFlag;		// 플래그 변수 enum ObjFlag 참조
};
END

#define __GAMEOBJECT_H__
#endif