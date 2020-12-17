#include "stdafx.h"
#include "..\Headers\EyebatBullet.h"
#include "Fire.h"
CEyebatBullet::CEyebatBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}

HRESULT CEyebatBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEyebatBullet::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = ((_vector*)pArg)[1];
		m_vLook = ((_vector*)pArg)[0] - m_pTransformCom->m_TransformDesc.vPosition;
		m_vLook.y = 0.f;
	}

	m_pTransformCom->m_TransformDesc.vScale = { 1.f, 1.f, 1.f };

	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 100;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fSpeed = 1.f;
	m_stOriginStatus.fImpact = 0.f;
	m_stStatus = m_stOriginStatus;

	m_fJumpPower = 15.f;
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;

	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 0;

	return S_OK;
}

_uint CEyebatBullet::UpdateGameObject(float fDeltaTime)
{
	//if (true == m_bTest)
	//	return 0;
	CBullet::UpdateGameObject(fDeltaTime);
	Movement(fDeltaTime);

	if (m_pTransformCom->m_TransformDesc.vPosition.y < 0) {
		m_byObjFlag ^= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
		CreateFire();
	}
	
	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CEyebatBullet::LateUpdateGameObject(float fDeltaTime)
{
	//if (true == m_bTest)
	//	return 0;
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CEyebatBullet::RenderGameObject()
{
	//if (true == m_bTest)
	//	return E_FAIL;
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	return S_OK;
}

HRESULT CEyebatBullet::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"EyebatBullet",
		L"Com_Texture_EyebatBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 2.5f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bMapCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

void CEyebatBullet::Movement(float fDeltaTime)
{
	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);
	m_fJumpTime += 0.01f;
	
	//m_pTransformCom->m_TransformDesc.vRotation.z += 5.f;
}

void CEyebatBullet::CreateFire()
{
	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CFire>(),
		(_int)ESceneID::Stage1st,
		CGameObject::Tag + TYPE_NAME<CFire>(),
		nullptr, (void*)&m_pTransformCom->m_TransformDesc.vPosition)))
		return;
	//m_bTest = true;
}

CEyebatBullet * CEyebatBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CEyebatBullet* pInstance = new CEyebatBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CEyebatBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEyebatBullet::Clone(void * pArg /*= nullptr*/)
{
	CEyebatBullet* pClone = new CEyebatBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CEyebatBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CEyebatBullet::Free()
{
	CBullet::Free();
}
