#include "stdafx.h"
#include "..\Headers\WeaponAmmoInfoUI.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "Layer.h"
#include "VIBuffer_UITexture.h"

CWeaponAmmoInfoUI::CWeaponAmmoInfoUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice)
{
	
}

HRESULT CWeaponAmmoInfoUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponAmmoInfoUI::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;
	
	m_UIDesc.vUISize.x = WINCX / 3.f;
	m_UIDesc.vUISize.y = WINCY / 2.f;
	//m_UIDesc.vUISize.y = WINCY / 1.f;
	m_UIDesc.vUISize.z = 0.f;

	m_UIDesc.vUIPos.x = WINCX / 2;
	m_UIDesc.vUIPos.y = -(WINCY / 2);
	m_UIDesc.vUIPos.z = 2.f;

	m_UIDesc.vCenter = _vector(1.f, -1.f, 0.f);

	return S_OK;
}

_uint CWeaponAmmoInfoUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);
	//ImGui::Begin("WeaponAmmoInfoUI Edit");

	//ImGui::Separator();
	//ImGui::SliderFloat3("Size",
	//	reinterpret_cast<float*>(&m_UIDesc.vUISize),
	//	-1000.f, +1000.f, "%f");

	//ImGui::Separator();
	//ImGui::SliderFloat3("Location",
	//	reinterpret_cast<float*>(&m_UIDesc.vUIPos),
	//	-1000.f, +2000.f, "%f");

	//ImGui::End();

	return _uint();
}

_uint CWeaponAmmoInfoUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this, ERenderPlace::FRONT)))
		return 0;

	return _uint();
}

HRESULT CWeaponAmmoInfoUI::RenderGameObject()
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

HRESULT CWeaponAmmoInfoUI::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_WeaponAmmoInfoUI",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CWeaponAmmoInfoUI * CWeaponAmmoInfoUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWeaponAmmoInfoUI* pInstance = new CWeaponAmmoInfoUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CWeaponAmmoInfoUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CWeaponAmmoInfoUI::Clone(void * pArg)
{
	CWeaponAmmoInfoUI* pClone = new CWeaponAmmoInfoUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CWeaponAmmoInfoUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CWeaponAmmoInfoUI::Free()
{
	CGameUI::Free();
}
