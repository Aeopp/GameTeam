#include "CollisionComponent.h"
#include "Management.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collision.h"
#include <d3dx9.h>
USING(Engine)

float  CCollisionComponent::MapCollisionCheckDistanceMin= 300.f;
float  CCollisionComponent::CollisionCheckDistanceMin = 300.f;
std::vector<CCollisionComponent*> CCollisionComponent::_Comps{};
int32_t CCollisionComponent::CurrentID{ 0 };
std::vector<PlaneInfo> CCollisionComponent::_MapPlaneInfo{};

std::map<CCollisionComponent::ETag, std::set<CCollisionComponent::ETag>> CCollisionComponent::_TagBind
{
	// 여기서 매칭되는 타입을 정의.
	{ MonsterAttack, { Player } },
	{ PlayerAttack, {  Monster} }
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
		// 버텍스 정보가 없을 경우?? <- 대부분 이경우에 해당.
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

	// 맵 평면과의 충돌
	if (bMapCollision)
	{
		for (const auto& _CurPlane : _CurMap)
		{
			vec3 ToPlaneCenter = _CurPlane.Center - _Sphere.Center;
			const float Distance = MATH::Length(ToPlaneCenter);
			if (Distance > MapCollisionCheckDistanceMin)continue;

			const auto CheckInfo = Collision::IsPlaneToSphere(_CurPlane, _Sphere);
			const bool bCurCollision = CheckInfo.first;
			const auto& CollisionInfo=CheckInfo.second;

			// 평면과는 일단 충돌한다.
			if (bCurCollision)
			{
				const vec3 ProjPt= MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
				// 평면과 일단 충돌한 상태에서
				// 구체의 중심을 평면에 투영한 이후의 점이 평면의 내부에 있다면 충돌.
				if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
				{
					MapHitProcess(CollisionInfo, _CurPlane);
					continue;
				}
				// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
				std::array<Segment,3ul> _Segments= MATH::MakeSegmentFromFace(_CurPlane.Face);
				for (const auto& _CurSegment : _Segments)
				{
					float t0 = 0;
					float t1 = 0;

					auto IsCollision=Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0,t1);
					if (IsCollision.first)
					{
						MapHitProcess(CollisionInfo,_CurPlane);
						continue;
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
//			PRINT_LOG(L"충돌체끼리 충돌!!", L"충돌체끼리 충돌!!");
		}
		else
		{
		//	PRINT_LOG(L"충돌체끼리 충돌하지않음!!", L"충돌체끼리 충돌하지않음!!");
		}
	}
}



void CCollisionComponent::DebugDraw()
{
	if (!CManagement::Get_Instance()->bDebug)return;

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);
	mat DebugSphereWorld = MATH::WorldMatrix({ 1,1,1 }, { 0,0,0 }, _Sphere.Center);
	m_pDevice->SetTransform(D3DTS_WORLD, &DebugSphereWorld);
	_SphereMesh->DrawSubset(0);



	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
};

void CCollisionComponent::Regist()
{
	_Comps.push_back(this);
}

void CCollisionComponent::CancelRegist()
{
	if (!_Comps.empty())
	{
		_Comps.erase(std::find_if(std::begin(_Comps), std::end(_Comps), [this](auto _Target)
			{
				return this->MyID == _Target->MyID;
			}));
	}
};

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

