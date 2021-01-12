#include "stdafx.h"
#include "..\Headers\SceneImage.h"
#include "UIManager.h"

USING(UI_AddTag)
CSceneImage::CSceneImage(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice)
{
}

HRESULT CSceneImage::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSceneImage::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	UI_BAR_ADD_COMPONENT tagInput = *static_cast<UI_BAR_ADD_COMPONENT*>(pArg);;

	if (FAILED(AddComponent(tagInput.wsPrototypeTag, tagInput.wsComponentTag)))
		return E_FAIL;

	m_UIDesc.vUISize.x = (float)WINCX;
	m_UIDesc.vUISize.y = (float)WINCY;
	m_UIDesc.vUISize.z = 0;

	m_UIDesc.vUIPos.x = 0.f;
	m_UIDesc.vUIPos.y = 0.f;
	m_UIDesc.vUIPos.z = 3.f;

	m_UIDesc.vCenter = _vector(0.f, 0.f, 0.f);

	m_bShown = true;
	return S_OK;
}

_uint CSceneImage::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CSceneImage::LateUpdateGameObject(float fDeltaTime)
{
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this, ERenderPlace::FRONT)))
		return 0;

	return _uint();
}

HRESULT CSceneImage::RenderGameObject()
{
	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_UIDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_UIDesc.matView)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_UIDesc.matOrthographic)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSceneImage::AddComponent(wstring _PrototypeTag, wstring _ComponentTag)
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		_PrototypeTag,
		_ComponentTag,
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CSceneImage * CSceneImage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSceneImage* pInstance = new CSceneImage(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CSceneImage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CSceneImage::Clone(void * pArg/* = nullptr*/)
{
	CSceneImage* pClone = new CSceneImage(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CSceneImage");
		SafeRelease(pClone);
	}

	return pClone;
}

void CSceneImage::Free()
{
	CGameUI::Free();
}
