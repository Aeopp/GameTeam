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

	m_UIDesc.matView._11 = m_UIDesc.vUISize.x;
	m_UIDesc.matView._22 = m_UIDesc.vUISize.y;
	m_UIDesc.matView._33 = 1.f;
	m_UIDesc.matView._41 = m_UIDesc.vUIPos.x;
	m_UIDesc.matView._42 = m_UIDesc.vUIPos.y;
	m_UIDesc.matView._43 = m_UIDesc.vUIPos.z;

	D3DXMatrixOrthoLH(&m_UIDesc.matOrthographic, WINCX, WINCY, 0.f, 1.f);

	return _uint();
}

HRESULT CGameUI::RenderGameObject()
{
	return S_OK;
}

HRESULT CGameUI::AddComponent()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Engine::CVIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CGameUI::Free()
{
	CGameObject::Free();
}
