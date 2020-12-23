#pragma once
#ifndef __COLLISIONCOMPONENT_H__

#include "Component.h"
#include "Collision.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionComponent final : public CComponent
{
protected:
	explicit CCollisionComponent(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCollisionComponent() = default;
public:
	using Super = CComponent;
	virtual HRESULT ReadyComponentPrototype() override;     
	virtual HRESULT ReadyComponent(void* pArg = nullptr/*InitInfo 구조체로 반드시 세팅*/) override;
public:
	static CCollisionComponent* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	void Update(class CTransform* const _Transform)&;
	void DebugDraw();
	void Regist();
	void CancelRegist();
	// 월드 공간으로 변환한 이후의 정보를 넘겨주기.
	static const std::vector<PlaneInfo>& GetMapPlaneInfo();
	FORCEINLINE static void CleanUpMapCollisionInfo()noexcept { _MapPlaneInfo.clear(); _MapFloorInfo.clear(); };
	static void AddMapPlaneInfo(const std::vector<PlaneInfo>& _MapPlaneInfo)noexcept;
	static void AddMapFloorInfo(const std::vector<PlaneInfo>& PMapFloorInfo)noexcept;
	static void CollisionUpdate(IDirect3DDevice9* const  _Device);
	static void CollisionDebugRender(IDirect3DDevice9* const  _Device);
	void MapHitProcess(const Collision::Info& CollisionInfo, const PlaneInfo& _CurPlane);
public:
	enum ETag : uint8_t
	{
		None,
		Player,
		PlayerAttack,
		Monster,
		MonsterAttack,
		Bullet,
		Effect,
		Item,
		Decorator,
		DestroyDecorator,
		Particle,
	};
public:
	struct InitInfo
	{
		float Radius = 10.f;
		bool bWallCollision = false;
		bool bFloorCollision = false;
		bool bCollision = true;
		bool bMapBlock = false;
		ETag Tag = ETag::None;
		class CGameObject* Owner = nullptr;
		// 버텍스 정보로 계산하고 싶다면 세팅
		vec3* Vertex=nullptr;
		DWORD FVF;
		DWORD VertexNumber;
	};
	bool bWallCollision = false;
	bool bFloorCollision = false;
	bool bMapBlock = false;
	bool bCollision = true;
	class CGameObject* Owner = nullptr;

	ID3DXMesh* _SphereMesh = nullptr;
	Sphere _Sphere;
	ETag _Tag = ETag::None;
	int32_t MyID= 0;
	static std::vector<PlaneInfo> _MapPlaneInfo; // 월드로 변환한 이후의 정보
	static std::vector<PlaneInfo> _MapFloorInfo; // 월드로 변환한 이후의 정보 (바닥)
	static float MapCollisionCheckDistanceMin;
	static float CollisionCheckDistanceMin;
	static std::vector<CCollisionComponent*> _Comps;
	static std::map<ETag, std::set<ETag>> _TagBind;
private:
	
	
	static int32_t CurrentID;


};

END

#define __COLLISIONCOMPONENT_H__
#endif

