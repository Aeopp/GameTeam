#include "stdafx.h"
#include "..\Headers\GameUI.h"
#include "Layer.h"

CGameUI::CGameUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{

}

HRESULT CGameUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameUI::ReadyGameObject(void * pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	return S_OK;
}

_uint CGameUI::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CGameUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);
	auto camera = m_pManagement->GetGameObject((int)ESceneID::Stage1st, CLayer::Tag + L"MainCamera", 0);
	if (nullptr == camera)
		return FALSE;

	TRANSFORM_DESC& tTransformDesc = m_pTransformCom->m_TransformDesc;

	//_matrix matScale;
	//_matrix matTrans;

	D3DXMatrixIdentity(&m_UIDesc.matWorld);
	D3DXMatrixIdentity(&m_UIDesc.matView);

	//D3DXMatrixTranslation(&m_UIDesc.matWorld, m_UIDesc.vUIPos.x, m_UIDesc.vUIPos.y, m_UIDesc.vUIPos.z);
	
	m_UIDesc.matWorld._11 = m_UIDesc.vUISize.x / 2;
	m_UIDesc.matWorld._22 = m_UIDesc.vUISize.y / 2;
	m_UIDesc.matWorld._33 = m_UIDesc.vUISize.z / 2;

	m_UIDesc.matWorld._41 = m_UIDesc.vUIPos.x - (m_UIDesc.vUISize.x * m_UIDesc.vCenter.x / 2);
	m_UIDesc.matWorld._42 = m_UIDesc.vUIPos.y - (m_UIDesc.vUISize.y * m_UIDesc.vCenter.y / 2);
	m_UIDesc.matWorld._43 = m_UIDesc.vUIPos.z;

	D3DXMatrixOrthoLH(&m_UIDesc.matOrthographic, WINCX, WINCY, 0.f, 5.f);

	return _uint();
}

HRESULT CGameUI::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameUI::AddComponent()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_VIBuffer_UITexture",
		L"Com_VIBufferUI",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CGameUI::Free()
{
	//SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	//SafeRelease(m_pTextureCom);		// 텍스처
	CGameObject::Free();
}
