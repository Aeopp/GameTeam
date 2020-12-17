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

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);

	m_matView._11 = m_vUISize.x;
	m_matView._22 = m_vUISize.y;
	m_matView._33 = 1.f;
	m_matView._41 = m_vUIPos.x;
	m_matView._42 = m_vUIPos.y;
	m_matView._43 = m_vUIPos.z;

	D3DXMatrixOrthoLH(&m_matOrthographic, WINCX, WINCY, 0.f, 1.f);

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
