#include "stdafx.h"
#include "..\Headers\FloorBlood.h"


CFloorBlood::CFloorBlood(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
{
}

HRESULT CFloorBlood::ReadyGameObjectPrototype()
{
	if (FAILED(CEffect::ReadyGameObjectPrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CFloorBlood::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CEffect::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
	}
	m_pTransformCom->m_TransformDesc.vRotation.x = 90.f;
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	m_fJumpPower = 15.f;
	m_fFrameCnt = rand() % 4;

	return S_OK;
}

_uint CFloorBlood::UpdateGameObject(float fDeltaTime)
{
	if (true == m_bTest)
		return 0;
	CEffect::UpdateGameObject(fDeltaTime);

	Jump(fDeltaTime);
	_CollisionComp->Update(m_pTransformCom);
	return _uint();
}

_uint CFloorBlood::LateUpdateGameObject(float fDeltaTime)
{
	if (false == m_bTest)
	{
		CEffect::LateUpdateGameObject(fDeltaTime);
		CEffect::IsBillboarding();
	}

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CFloorBlood::RenderGameObject()
{
	if (FAILED(CEffect::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	return S_OK;
}

void CFloorBlood::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	m_bTest = true;
	m_pTransformCom->m_TransformDesc.vPosition.y -= 3.f;
}

HRESULT CFloorBlood::AddComponents()
{
	if (FAILED(CEffect::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"FloorBlood",
		L"Com_Texture_FloorBlood",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.0f;
	_Info.Tag = CCollisionComponent::ETag::Particle;
	_Info.bFloorCollision = true;
	_Info.bWallCollision = false;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

void CFloorBlood::Jump(float fDeltaTime)
{
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);
	m_fJumpTime += 0.05f;
}

CGameObject * CFloorBlood::Clone(void * pArg /*= nullptr*/)
{
	CFloorBlood* pClone = new CFloorBlood(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacierParticle");
		SafeRelease(pClone);
	}

	return pClone;
}

CFloorBlood * CFloorBlood::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CFloorBlood* pInstance = new CFloorBlood(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CFloorBloodPrototype");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CFloorBlood::Free()
{
	CEffect::Free();
}
