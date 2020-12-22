#include "stdafx.h"
#include "..\Headers\GlacierParticle.h"

CGlacierParticle::CGlacierParticle(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
{
}

HRESULT CGlacierParticle::ReadyGameObjectPrototype()
{
	if (FAILED(CEffect::ReadyGameObjectPrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlacierParticle::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CEffect::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
	}

	//m_pTransformCom->m_TransformDesc.vPosition = { 10.f,10.f,20.f };
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	m_fJumpPower = 15.f;
	m_pTransformCom->m_TransformDesc.vScale = { 0.5f,0.5f,0.5f };
	int iRandomX = rand() % 10 - 5;
	int iRandomZ = rand() % 10 - 5;

	m_vLook = { (float)iRandomX,0.f,(float)iRandomZ };
	m_fFrameCnt = rand() % 5;


	return S_OK;
}

_uint CGlacierParticle::UpdateGameObject(float fDeltaTime)
{

	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);
	m_fJumpTime += 0.03f;
	CEffect::UpdateGameObject(fDeltaTime);

	//_CollisionComp->bMapCollision = true;
	_CollisionComp->Update(m_pTransformCom);
	return _uint();
}

_uint CGlacierParticle::LateUpdateGameObject(float fDeltaTime)
{
	CEffect::LateUpdateGameObject(fDeltaTime);

	CEffect::IsBillboarding();

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;



	return _uint();
}

HRESULT CGlacierParticle::RenderGameObject()
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

void CGlacierParticle::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	int i = 0;
}

HRESULT CGlacierParticle::AddComponents()
{
	if (FAILED(CEffect::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"GlacierParticle",
		L"Com_Texture_GlacierParticle",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 2.5f;
	_Info.Tag = CCollisionComponent::ETag::Particle;
	_Info.bMapCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

CGlacierParticle * CGlacierParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacierParticle* pInstance = new CGlacierParticle(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacierParticle");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CGlacierParticle::Clone(void * pArg /*= nullptr*/)
{
	CGlacierParticle* pClone = new CGlacierParticle(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacierParticle");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacierParticle::Free()
{
	CEffect::Free();
}
