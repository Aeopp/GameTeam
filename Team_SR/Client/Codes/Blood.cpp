#include "stdafx.h"
#include "..\Headers\Blood.h"
CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
{
}

HRESULT CBlood::ReadyGameObjectPrototype()
{
	if (FAILED(CEffect::ReadyGameObjectPrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBlood::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CEffect::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
	}
	//테스트
	m_pTransformCom->m_TransformDesc.vPosition.x += (rand() % 3 - 1)/5.f;
	m_pTransformCom->m_TransformDesc.vPosition.y += (rand() % 3 - 1) / 5.f;
	m_pTransformCom->m_TransformDesc.vPosition.z -= 1;
	//m_pTransformCom->m_TransformDesc.vScale = { 0.5f,0.5f,0.5f };
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 7.f;
	m_fFrameSpeed = 10.f;
	return S_OK;
}

_uint CBlood::UpdateGameObject(float fDeltaTime)
{
	CEffect::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CBlood::LateUpdateGameObject(float fDeltaTime)
{
	CEffect::LateUpdateGameObject(fDeltaTime);

	CEffect::IsBillboarding();

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	if (true == m_bFrameLoopCheck)
		m_byObjFlag |= (BYTE)ObjFlag::Remove;

	return _uint();
}

HRESULT CBlood::RenderGameObject()
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

CBlood * CBlood::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CBlood* pInstance = new CBlood(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CBlood");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CBlood::Clone(void * pArg /*= nullptr*/)
{
	CBlood* pClone = new CBlood(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CBlood");
		SafeRelease(pClone);
	}

	return pClone;
}

void CBlood::Free()
{
	CEffect::Free();
}

HRESULT CBlood::AddComponents()
{
	if (FAILED(CEffect::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTextureBlood = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CBlood>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureBlood,
		L"Com_Texture_Blood",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}
