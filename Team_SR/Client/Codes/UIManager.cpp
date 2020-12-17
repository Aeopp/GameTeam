#include "stdafx.h"
#include "..\Headers\UIManager.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
	: m_pPlayerInfoUI(nullptr), m_pWeaponAmmoInfoUI(nullptr)
{
}

HRESULT CUIManager::ReadyUI()
{
	CManagement* pManagement = CManagement::Get_Instance();

	// 플레이어 정보 - 체력, 마나, 초상화
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_PlyerInfoUI",
		(_int)ESceneID::Stage1st,
		L"Layer_GameObject_PlyerInfoUI",
		(CGameObject**)&m_pPlayerInfoUI, nullptr)))
		return E_FAIL;

	// 무기 탄약 수
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_WeaponAmmoInfoUI",
		(_int)ESceneID::Stage1st,
		L"Layer_WeaponAmmoInfoUI",
		(CGameObject**)&m_pWeaponAmmoInfoUI, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CUIManager::OnAllUI()
{
}

void CUIManager::OffAllUI()
{
}

void CUIManager::OnPlayerInfo()
{
}

void CUIManager::OffPlayerInfo()
{
}

void CUIManager::OnWeaponAmmom()
{
}

void CUIManager::OffWeaponAmmom()
{
}

void CUIManager::Free()
{
}
