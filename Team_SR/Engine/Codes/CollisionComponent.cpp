#include "CollisionComponent.h"
#include "Management.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collision.h"
#include <d3dx9.h>
#pragma warning(disable:4819)

USING(Engine)

float  CCollisionComponent::MapCollisionCheckDistanceMin= 60.f;
float  CCollisionComponent::CollisionCheckDistanceMin = 60.f;
std::vector<CCollisionComponent*> CCollisionComponent::_Comps{};
int32_t CCollisionComponent::CurrentID{ 0 };
std::vector<PlaneInfo> CCollisionComponent::_MapPlaneInfo{};
std::vector<PlaneInfo> CCollisionComponent::_MapFloorInfo{};

std::map<CCollisionComponent::ETag, std::set<CCollisionComponent::ETag>> CCollisionComponent::_TagBind
{
	{ MonsterAttack, { Player } },
	{ PlayerAttack, {  Monster} },
	{Item,{Player} }
};

CCollisionComponent::CCollisionComponent(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{

}

HRESULT CCollisionComponent::ReadyComponentPrototype()
{
	if (FAILED( Super::ReadyComponent()) )
		return E_FAIL;

	return S_OK;
}

HRESULT CCollisionComponent::ReadyComponent(void* pArg)
{
	if (FAILED(Super::ReadyComponent(pArg)))
		return E_FAIL;

	if (nullptr==pArg)
	{
		PRINT_LOG(L"Warning!", __FUNCTIONW__);
		return E_FAIL;
	}

	Regist();

	InitInfo _Info = *reinterpret_cast<InitInfo*>(pArg);
	
	bFloorCollision = _Info.bFloorCollision;
	bWallCollision = _Info.bWallCollision;
	bMapBlock = _Info.bMapBlock;
	bCollision = _Info.bCollision;
	bMapBlock = _Info.bMapBlock;
	_Sphere.Radius = _Info.Radius;
	_Tag = _Info.Tag;
	Owner = _Info.Owner;
	if (_Info.Vertex)
	{
		D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)_Info.Vertex,
			_Info.VertexNumber,
			D3DXGetFVFVertexSize(_Info.FVF),
			&_Sphere.Center,
			&_Sphere.Radius);
	}
	else
	{
		
	}

	D3DXCreateSphere(m_pDevice, _Sphere.Radius, 25, 25, &_SphereMesh, 0);

	return S_OK;
}


CCollisionComponent* CCollisionComponent::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollisionComponent* pInstance = new CCollisionComponent(pDevice);

	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CCollisionComponent");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent* CCollisionComponent::Clone(void* pArg)
{
	CCollisionComponent* pClone = new CCollisionComponent(*this);
	pClone->MyID= CurrentID++;
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CCollisionComponent");
		SafeRelease(pClone);
	}
	return pClone;
}

void CCollisionComponent::Free()
{
	SafeRelease(_SphereMesh);
	CancelRegist();
	Super::Free();
};


void CCollisionComponent::Update(class CTransform* const _Transform)&
{
	_Sphere.Center = _Transform->m_TransformDesc.vPosition;
	if (!bCollision)return;

	const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;
	
	if (bWallCollision)
	{
		for (const auto& _CurPlane : _CurMap)
		{
			vec3 ToPlaneCenter = _CurPlane.Center - _Sphere.Center;
			const float Distance = MATH::Length(ToPlaneCenter);
			if (Distance > MapCollisionCheckDistanceMin)continue;

			 auto CheckInfo = Collision::IsPlaneToSphere(_CurPlane, _Sphere);
			const bool bCurCollision = CheckInfo.first;
			 auto& CollisionInfo=CheckInfo.second;

			// 평면과는 일단 충돌한다.
			if (bCurCollision)
			{
				const vec3 ProjPt= MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
				// 평면과 일단 충돌한 상태에서
				// 구체의 중심을 평면에 투영한 이후의 점이 평면의 내부에 있다면 충돌.
				if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
				{
					CollisionInfo.Flag = L"Wall";
					CollisionInfo.IntersectPoint = ProjPt;
					MapHitProcess(CollisionInfo, _CurPlane);
					continue;
				}
				// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
				std::array<Segment,3ul> _Segments= MATH::MakeSegmentFromFace(_CurPlane.Face);
				for (const auto& _CurSegment : _Segments)
				{
					float t0 = 0;
					float t1 = 0;
					vec3 IntersectPoint;
					auto IsCollision=Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0,t1 , IntersectPoint);
					if (IsCollision.first)
					{
						CollisionInfo.Flag = L"Wall";
						CollisionInfo.IntersectPoint = IntersectPoint;
						MapHitProcess(CollisionInfo,_CurPlane );
						continue;
					}
				}
			}
		}
	}

	{
		const auto& _CurMapFloor = CCollisionComponent::_MapFloorInfo;

		if (bFloorCollision)
		{
			for (const auto& _CurPlane : _CurMapFloor)
			{
                 // 벽과의 충돌의 차이점은 바닥은 현재 최적화로
				// 삼각형 두개 만으로 만들었기 때문에 중심점이 매우 멀수 있어서
				// 거리검사로 컬링을 하지 않는다.
				auto CheckInfo = Collision::IsPlaneToSphere(_CurPlane, _Sphere);
				const bool bCurCollision = CheckInfo.first;
				 auto& CollisionInfo = CheckInfo.second;

				// 평면과는 일단 충돌한다.
				if (bCurCollision)
				{
					const vec3 ProjPt = MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
					// 평면과 일단 충돌한 상태에서
					// 구체의 중심을 평면에 투영한 이후의 점이 평면의 내부에 있다면 충돌.
					if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
					{
						CollisionInfo.Flag = L"Floor";
						CollisionInfo.IntersectPoint = ProjPt;
						MapHitProcess(CollisionInfo, _CurPlane);
						continue;
					}
					// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
					std::array<Segment, 3ul> _Segments = MATH::MakeSegmentFromFace(_CurPlane.Face);
					for (const auto& _CurSegment : _Segments)
					{
						vec3 IntersectPoint; 
						float t0 = 0;
						float t1 = 0;

						auto IsCollision = Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0, t1, IntersectPoint);
						if (IsCollision.first)
						{
							CollisionInfo.Flag = L"Floor";
							CollisionInfo.IntersectPoint = IntersectPoint;
							MapHitProcess(CollisionInfo, _CurPlane);
							continue;
						}
					}
				}
			}
		}
	}

	// 충돌체 끼리의 충돌
	for (auto& _Comp : _Comps)
	{
#pragma region MatchingCheck
		if (this == _Comp)continue;
		auto iter = _TagBind.find(this->_Tag);
		if (iter == std::end(_TagBind))continue;
		if (iter->second.find(_Comp->_Tag) == std::end(iter->second))continue;
		vec3 ToRhs = _Sphere.Center - _Comp->_Sphere.Center;
#pragma endregion
		// 거리검사
		if (MATH::Length(ToRhs) > MapCollisionCheckDistanceMin)continue;
		// ....
		auto IsCollision = Collision::IsSphereToSphere(_Sphere, _Comp->_Sphere);
		// 충돌함.
		if (IsCollision.first)
		{
			auto* _LhsOwner = this->Owner;
			auto* _RhsOwner = _Comp->Owner;

			_LhsOwner->Hit(_RhsOwner, IsCollision.second);
			_RhsOwner->Hit(_LhsOwner, IsCollision.second);
			 //PRINT_LOG(L"충돌체끼리 충돌!!", L"충돌체끼리 충돌!!");
		}
		else
		{
			// PRINT_LOG(L"충돌체끼리 충돌하지않음!!", L"충돌체끼리 충돌하지않음!!");
		}
	}
}



void CCollisionComponent::DebugDraw()
{
	if (!CManagement::Get_Instance()->bDebug)return;

	DWORD _AlphaValue;
	m_pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &_AlphaValue);
	if (_AlphaValue == TRUE)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);
	mat DebugSphereWorld = MATH::WorldMatrix({ 1,1,1 }, { 0,0,0 }, _Sphere.Center);
	m_pDevice->SetTransform(D3DTS_WORLD, &DebugSphereWorld);
	_SphereMesh->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (_AlphaValue == TRUE)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
};
 const std::vector<PlaneInfo>& CCollisionComponent::GetMapPlaneInfo()
 { 
	 return _MapPlaneInfo; 
 };


void CCollisionComponent::Regist()
{
	_Comps.push_back(this);
}

void CCollisionComponent::CancelRegist()
{
	if (!_Comps.empty())
	{
		auto EraseTargetIter = std::find_if(std::begin(_Comps), std::end(_Comps), [this](auto _Target)
			{
				return this->MyID == _Target->MyID;
			});

		if (EraseTargetIter != std::end(_Comps))
		{
			_Comps.erase(EraseTargetIter);
		}		
	}
};


// 월드 공간으로 변환한 이후의 정보를 넘겨주기.

void CCollisionComponent::AddMapPlaneInfo(const std::vector<PlaneInfo>& _MapPlaneInfo) noexcept 
{
	CCollisionComponent::_MapPlaneInfo.insert( std::end(CCollisionComponent::_MapPlaneInfo ) , std::begin(_MapPlaneInfo), 	std::end(_MapPlaneInfo));
}

void CCollisionComponent::AddMapFloorInfo(const std::vector<PlaneInfo>& PMapFloorInfo) noexcept
{
	CCollisionComponent::_MapFloorInfo.insert(std::end(CCollisionComponent::_MapFloorInfo), std::begin(PMapFloorInfo), std::end(PMapFloorInfo));
}

void CCollisionComponent::CollisionUpdate(IDirect3DDevice9* const  _Device)
{

}

void CCollisionComponent::CollisionDebugRender(IDirect3DDevice9*const  _Device)
{

}

void CCollisionComponent::MapHitProcess(const Collision::Info& CollisionInfo,const PlaneInfo& _CurPlane)
{
	if (bMapBlock)
	{
		Owner->GetTransform()->m_TransformDesc.vPosition += CollisionInfo.Dir * CollisionInfo.CrossValue;
	}

	Owner->MapHit(_CurPlane, CollisionInfo);
}

