#include "stdafx.h"
#include "..\Headers\WeaponInfoUI.h"
#include "ImGuiHelper.h"


CWeaponInfoUI::CWeaponInfoUI(LPDIRECT3DDEVICE9 pDevice)
	:CGameUI(pDevice)
{
}

HRESULT CWeaponInfoUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponInfoUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	//if (nullptr == pArg)
	//	return E_FAIL;

	return S_OK;
}

_uint CWeaponInfoUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);
	if (!m_bShown)
		return S_OK;
	ImGui::Begin("Weapon Info UI");


	ImGui::Separator();
	ImGui::SliderFloat3("Size",
		reinterpret_cast<float*>(&m_UIDesc.vUISize),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_UIDesc.vUIPos),
		-1000.f, +1000.f, "%f");
	return _uint();
}

_uint CWeaponInfoUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;

	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CWeaponInfoUI::RenderGameObject()
{
	if (!m_bShown)
		return S_OK;

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

CWeaponInfoUI * CWeaponInfoUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWeaponInfoUI* pInstance = new CWeaponInfoUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CWeaponInfoUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CWeaponInfoUI::Clone(void * pArg/* = nullptr*/)
{
	CWeaponInfoUI* pClone = new CWeaponInfoUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CWeaponInfoUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CWeaponInfoUI::Free()
{
	CGameUI::Free();
}
