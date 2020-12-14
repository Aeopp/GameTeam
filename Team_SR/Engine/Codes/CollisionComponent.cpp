#include "CollisionComponent.h"
#include "Management.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collision.h"
#include <d3dx9.h>

USING(Engine)

float  CCollisionComponent::MapCollisionCheckDistanceMin= 100.f;
float  CCollisionComponent::CollisionCheckDistanceMin = 100.f;
std::vector<CCollisionComponent*> CCollisionComponent::_Comps{};
int32_t CCollisionComponent::CurrentID{ 0 };
std::vector<PlaneInfo> CCollisionComponent::_MapPlaneInfo{};
std::map<CCollisionComponent::ETag, std::set<CCollisionComponent::ETag>> CCollisionComponent::_TagBind
{
	{ MonsterAttack, { Player } },
	{ PlayerAttack, {  Monster} }
};

// TOOD:: REMOVE 
//mat DebugMat;
//std::array<vec3,4ul> VertexList
//{
//	vec3{-1,1, 0},
//	vec3{1,-1,0},
//	vec3{-1,-1,0},
//	vec3{-1,1, 0}
//};
//vec3 location{ 0,0,0 };
//vec3 scale{ 5,5,5 };
//vec3 rotation{ 0,0,0};
// 

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
}

void CCollisionComponent::Update(class CTransform* const _Transform)&
{
	_Sphere.Center = _Transform->m_TransformDesc.vPosition;
	if (!bCollision)return;

	const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

	// TODO:: REMOVE
	//{
	//	DebugMat=MATH::WorldMatrix(scale, rotation, location);

	//	vec3 Center{ 0,0,0 };
	//	size_t i = 0;
	//	PlaneInfo _Info;
	//	for (const auto& Vertex : VertexList)
	//	{
	//		vec3 world_location;
	//		world_location = MATH::Mul(Vertex, DebugMat);
	//		Center += world_location;
	//		_Info.Face[i++] = world_location;
	//		if (i == 3)break;
	//	}
	//	_Info.Center = Center / 3.f;
	//	D3DXPlaneFromPoints(&_Info._Plane,
	//		&_Info.Face[0], &_Info.Face[1], &_Info.Face[2]);

	//	_MapPlaneInfo.clear();
	//	_MapPlaneInfo.push_back(_Info);
	//	D3DXPlaneFromPoints(&_Info._Plane,
	//		&_Info.Face[2], &_Info.Face[1], &_Info.Face[0]);
	//	_MapPlaneInfo.push_back(_Info);
	//}
	//

	// �� ������ �浹
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

			// ������ �ϴ� �浹�Ѵ�.
			if (bCurCollision)
			{

				const vec3 ProjPt= MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
				// ���� �ϴ� �浹�� ���¿���
				// ��ü�� �߽��� ��鿡 ������ ������ ���� ����� ���ο� �ִٸ� �浹.
				if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
				{
					MapHitProcess(CollisionInfo, _CurPlane);
					continue;
				}
				// �ﰢ������ ���� 3���� ������ ���Ŀ� ���а� ���� �浹�� �˻��Ѵ�.
				std::array<Segment,3ul> _Segments= MATH::MakeSegmentFromFace(_CurPlane.Face);
				for (const auto& _CurSegment : _Segments)
				{
					float t = 0;
					auto IsCollision=Collision::IsSegmentToSphere(_CurSegment, _Sphere, t);
					if (IsCollision.first)
					{
						MapHitProcess(CollisionInfo,_CurPlane);
						continue;
					}
				}
			}
		}
	}

	// �浹ü ������ �浹
	for (auto& _Comp : _Comps)
	{
#pragma region MatchingCheck
		if (this == _Comp)continue;
		auto iter = _TagBind.find(this->_Tag);
		if (iter == std::end(_TagBind))continue;
		if (iter->second.find(_Comp->_Tag) == std::end(iter->second))continue;
		vec3 ToRhs = _Sphere.Center - _Comp->_Sphere.Center;
#pragma endregion
		// �Ÿ��˻�
		if (MATH::Length(ToRhs) > MapCollisionCheckDistanceMin)continue;
		// ....
		auto IsCollision = Collision::IsSphereToSphere(_Sphere, _Comp->_Sphere);
		// �浹��.
		if (IsCollision.first)
		{
			PRINT_LOG(L"�浹ü���� �浹!!", L"�浹ü���� �浹!!");
		}
		else
		{
			
		}
	}
}


void CCollisionComponent::DebugDraw()
{
	if (!CManagement::Get_Instance()->bDebug)return;

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// TODO :: REMOVE
	//auto& _DXLine = CManagement::Get_Instance()->GetDXLine();
	//mat view, proj;
	//m_pDevice->GetTransform(D3DTS_VIEW, &view);
	//DebugMat *= view;
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	//DebugMat *= proj;
	//_DXLine.DrawTransform(VertexList.data(), 4, &DebugMat, D3DCOLOR_XRGB(255, 255, 255));
	//

	D3DMATERIAL9 _Mtrl;

	_Mtrl.Ambient = D3DCOLORVALUE{ 1,1,1,1 };
	_Mtrl.Diffuse = D3DCOLORVALUE{ 1,1,1,1 };
	_Mtrl.Emissive = D3DCOLORVALUE{ 1,1,1,1 };
	_Mtrl.Power = 10.f;
	_Mtrl.Specular = D3DCOLORVALUE{ 1,1,1,1 };
	m_pDevice->SetMaterial(&_Mtrl);
	_SphereMesh->DrawSubset(0);

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
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
}


void CCollisionComponent::MapHitProcess(const Collision::Info& CollisionInfo,const PlaneInfo& _CurPlane)
{
	if (bMapBlock)
	{
		Owner->GetTransform()->m_TransformDesc.vPosition += CollisionInfo.Dir * CollisionInfo.CrossValue;
	}

	Owner->MapHit(_CurPlane, CollisionInfo);
}

