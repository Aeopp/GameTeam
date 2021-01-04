#include "stdafx.h"
#include "..\Headers\SpiderBullet.h"

CSpiderBullet::CSpiderBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}

HRESULT CSpiderBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderBullet::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_fFrameCnt = 0;

	return S_OK;
}

_uint CSpiderBullet::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CSpiderBullet::LateUpdateGameObject(float fDeltaTime)
{

	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;


	return _uint();
}

HRESULT CSpiderBullet::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	return S_OK;
}

HRESULT CSpiderBullet::AddComponents()
{

	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	// 글레이서 불렛 텍스처
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"SpiderWep",
		L"Com_Texture_SpiderWep",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

CSpiderBullet * CSpiderBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSpiderBullet* pInstance = new CSpiderBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CSpiderBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CSpiderBullet::Clone(void * pArg/* = nullptr*/)
{
	CSpiderBullet* pClone = new CSpiderBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CSpiderBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CSpiderBullet::Free()
{
	CBullet::Free();
}
