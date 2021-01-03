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
	//auto camera = m_pManagement->GetGameObject((int)-1, CLayer::Tag + L"MainCamera", 0);
	//if (nullptr == camera)
	//	return FALSE;

	SetupUIMatrix(m_UIDesc);

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

_uint CGameUI::SetupUIMatrix(UI_DESC & _desc)
{
	D3DXMatrixIdentity(&_desc.matWorld);
	D3DXMatrixIdentity(&_desc.matView);

	//D3DXMatrixTranslation(&m_UIDesc.matWorld, m_UIDesc.vUIPos.x, m_UIDesc.vUIPos.y, m_UIDesc.vUIPos.z);

	_desc.matWorld._11 = _desc.vUISize.x / 2;
	_desc.matWorld._22 = _desc.vUISize.y / 2;
	_desc.matWorld._33 = _desc.vUISize.z / 2;
						 
	_desc.matWorld._41 = _desc.vUIPos.x - (_desc.vUISize.x * _desc.vCenter.x / 2);
	_desc.matWorld._42 = _desc.vUIPos.y - (_desc.vUISize.y * _desc.vCenter.y / 2);
	_desc.matWorld._43 = _desc.vUIPos.z;

	D3DXMatrixOrthoLH(&_desc.matOrthographic, WINCX, WINCY, 0.f, 5.f);

	return _uint();
}

void CGameUI::Free()
{
	//SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	//SafeRelease(m_pTextureCom);		// 텍스처
	CGameObject::Free();
}
