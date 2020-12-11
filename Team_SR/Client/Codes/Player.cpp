#include "stdafx.h"
#include "..\Headers\Player.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "DXWrapper.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{}

HRESULT CPlayer::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.fSpeedPerSec = 50;
	m_pTransformCom->m_TransformDesc.fRotatePerSec = MATH::PI;
	m_pTransformCom->m_TransformDesc.vPosition = { 0,10,0 };
	m_pTransformCom->m_TransformDesc.vRotation = { 0,0,0 };
	m_pTransformCom->m_TransformDesc.vScale = { 1,1,1 };

	_SpotLight = Light::GetSpot(m_pTransformCom->GetLocation(),
		m_pTransformCom->GetLook(), Color::WHITE);


	return S_OK;
}

_uint CPlayer::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("PlayerEdit", &ImGuiHelper::bEditOn);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("Pitch", &m_pTransformCom->m_TransformDesc.vRotation.x, -360, 360, "Deg : %f");
		ImGui::SliderFloat("Yaw", &m_pTransformCom->m_TransformDesc.vRotation.y, -360, 360, "Deg : %f");
		ImGui::SliderFloat("Roll", &m_pTransformCom->m_TransformDesc.vRotation.z, -360, 360, "Deg : %f");

		ImGui::Text("Scale");
		ImGui::Separator();
		ImGui::SliderFloat("x", &m_pTransformCom->m_TransformDesc.vScale.x, 0.1, 100.f);
		ImGui::SliderFloat("y", &m_pTransformCom->m_TransformDesc.vScale.y, 0.1, 100.f);
		ImGui::SliderFloat("z", &m_pTransformCom->m_TransformDesc.vScale.z, 0.1, 100.f);


		ImGui::Separator();
		ImGui::Text("Information");
		ImGui::Separator();
		ImGui::Text("Location");
		const auto& Location = m_pTransformCom->m_TransformDesc.vPosition;
		ImGui::Text("X : %f", Location.x);
		ImGui::Text("Y : %f", Location.y);
		ImGui::Text("Z : %f", Location.z);

		ImGui::Text("Collision");
		ImGui::Checkbox("Collision?", &_CollisionComp->bCollision);

		ImGui::End();
	}
	

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CPlayer::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	_SpotLight = Light::GetSpot(m_pTransformCom->GetLocation(),
		m_pTransformCom->GetLook(), Color::WHITE);

	m_pManagement->RegistLight(_SpotLight);

	return _uint();
}

HRESULT CPlayer::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	_CollisionComp->DebugDraw();

	return S_OK;
}

//_vector vLook;
//memcpy(&vLook, &m_TransformDesc.matWorld.m[2][0], sizeof(_vector));
//D3DXVec3Normalize(&vLook, &vLook);
//
//m_TransformDesc.vPosition += vLook * m_TransformDesc.fSpeedPerSec * fDeltaTime;
//}
//
//void CTransform::GoSide(float fDeltaTime)
//{
//	_vector vRight;
//	memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(_vector));
//	D3DXVec3Normalize(&vRight, &vRight);
//
//	m_TransformDesc.vPosition += vRight * m_TransformDesc.fSpeedPerSec * fDeltaTime;
//}
void CPlayer::MoveForward(const float DeltaTime)&
{
	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Forward{ world._31,0.f,world._33 };
	Forward = MATH::Normalize(Forward);
	const float Speed = Desc.fSpeedPerSec;
	Desc.vPosition += Forward * Speed * DeltaTime;
};

void CPlayer::MoveRight(const float DeltaTime)&
{
	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Right{ world._11,0.f,world._13 };
	Right = MATH::Normalize(Right);
	const float Speed = Desc.fSpeedPerSec;
	Desc.vPosition += Right * Speed * DeltaTime;
};

HRESULT CPlayer::AddStaticComponents()
{
	Super::AddStaticComponents();

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Player;
	_Info.bMapCollision = true;
	_Info.Owner = this;

	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}


CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Player");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pClone = new CPlayer(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone Player");
		SafeRelease(pClone);
	}

	return pClone;
}

void CPlayer::Free()
{
	SafeRelease(_CollisionComp);

	CGameObject::Free();
}
