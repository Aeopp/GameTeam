#include "stdafx.h"
#include "..\Headers\UIManager.h"
#include "Management.h"
#include "VIBuffer_UITexture.h"
#include "PlayerInfoUI.h"
#include "WeaponAmmoInfoUI.h"
#include "LoadingBar.h"
#include "HUDTopUI.h"
#include "HUDTopUI.h"

IMPLEMENT_SINGLETON(CUIManager)

using namespace UI_AddTag;
CUIManager::CUIManager()
	: m_pPlayerInfoUI(nullptr), m_pWeaponAmmoInfoUI(nullptr)
{
}

HRESULT CUIManager::ReadyUI()
{
	m_pDevice =  CManagement::Get_Instance()->GetDevice();
	SafeAddRef(m_pDevice);

	CManagement* pManagement = CManagement::Get_Instance();

	//Add Prototype
#pragma region Add_Prototype
	//Ojbect
	//Componet
	//Texture
	// 플레이어 UI 텍스처
#pragma region Component_Texture_PlayerInfoUI
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_PlayerInfoUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bottom_left.png", 1))))
		return E_FAIL;
#pragma endregion

	// 무기 탄약 UI 텍스처
#pragma region Component_Texture_WeaponAmmoInfoUI
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_WeaponAmmoInfoUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bottom_right.png", 1))))
		return E_FAIL;
#pragma endregion

	//HP_Bar
#pragma region Component_Texture_HPbar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_HPbar",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bar_HP.png", 1))))
		return E_FAIL;
#pragma endregion

	//Mana_Bar
#pragma region Component_Texture_Mana_Bar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Manabar",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bar_mana.png", 1))))
		return E_FAIL;
#pragma endregion

	//Ammo_Bar
#pragma region Component_Texture_AmmoBar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_AmmoBar",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bar_ammo.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_HUDBossBar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_HUDTopUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_top.png", 1))))
		return E_FAIL;
#pragma endregion
	
	//Buffer
#pragma region Component_VIBuffer_UITexture
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_UITexture>(),
		CVIBuffer_UITexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
#pragma endregion

#pragma region Add_GameObject
#pragma region GameObject_LoadingBar
	if (FAILED(pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CLoadingBar>(),
		CLoadingBar::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
#pragma region GameObject_HUDBossBar
	if (FAILED(pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CHUDTopUI>(),
		CHUDTopUI::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
#pragma endregion

	//Add Layer
#pragma region Add_Layer
	//플레이어 정보 - 체력, 마나, 초상화
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_PlayerInfoUI",
		(_int)ESceneID::Static,
		L"Layer_GameObject_PlayerInfoUI",
		(CGameObject**)&m_pPlayerInfoUI, nullptr)))
		return E_FAIL;

	// 무기 탄약 수
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_WeaponAmmoInfoUI",
		(_int)ESceneID::Static,
		L"Layer_WeaponAmmoInfoUI",
		(CGameObject**)&m_pWeaponAmmoInfoUI, nullptr)))
		return E_FAIL;

	// 상단UI / 보스 체력
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_HUDTopUI",
		(_int)ESceneID::Static,
		L"Layer_HUDTopUI",
		(CGameObject**)&m_pHUD_TopUI, nullptr)))
		return E_FAIL;

//#pragma region Test_Player
//
//	m_iMaxs[0] = 100;
//	m_iMaxs[1] = 200;
//	m_iMaxs[2] = 50;
//		  
//	m_iMins[0] = 100;
//	m_iMins[1] = 100;
//	m_iMins[2] = 100;
//
//	m_pHUD_HpBar->SetMaxValueAndMinValue(&m_iMaxs[0], &m_iMins[0]);
//	m_pHUD_ManaBar->SetMaxValueAndMinValue(&m_iMaxs[1], &m_iMins[1]);
//	m_pHUD_AmmoBar->SetMaxValueAndMinValue(&m_iMaxs[2], &m_iMins[2]);
//#pragma endregion
//#pragma region Test_MonsterHP
//	m_iMax = 100;
//	m_iMin = 100;
//
//	OnMonsterBar(&m_iMax, &m_iMin);
//#pragma endregion
	return S_OK;
}

void CUIManager::UIOpen(ESceneID SceneID)
{
	CManagement* pManagement = CManagement::Get_Instance();
	//------------------------------------------------------
	UI_BAR_ADD_COMPONENT tagLayerCom;
#pragma region HUD_HP_Bar
	//HUD_HP_Bar
	tagLayerCom.tUIDesc.vUISize.x = 312.f;
	tagLayerCom.tUIDesc.vUISize.y = 60.f;
	tagLayerCom.tUIDesc.vUISize.z = 0;
	tagLayerCom.tUIDesc.vUIPos.x = -742;
	tagLayerCom.tUIDesc.vUIPos.y = -410;
	tagLayerCom.tUIDesc.vUIPos.z = 0.f;
	tagLayerCom.tUIDesc.vCenter = _vector(-1.f, 0.f, 0.f);
	tagLayerCom.wsPrototypeTag = L"Component_Texture_HPbar";
	tagLayerCom.wsComponentTag = L"Com_Texture";
	tagLayerCom.bTextOut = true;

	//HUD_HP_Bar
	pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)SceneID,
		L"Layer_HUD_HpBar",
		(CGameObject**)&m_pHUD_HpBar, &tagLayerCom);

#pragma endregion
#pragma region HUD_Mana_Bar
	//HUD_Mana_Bar
	tagLayerCom.tUIDesc.vUISize.x = 310.f;
	tagLayerCom.tUIDesc.vUISize.y = 60.f;
	tagLayerCom.tUIDesc.vUISize.z = 0.f;
	tagLayerCom.tUIDesc.vUIPos.x = -675.f;
	tagLayerCom.tUIDesc.vUIPos.y = -480.f;
	tagLayerCom.tUIDesc.vUIPos.z = 1.f;
	tagLayerCom.tUIDesc.vCenter = _vector(-1.f, 0.f, 0.f);
	tagLayerCom.wsPrototypeTag = L"Component_Texture_Manabar";
	tagLayerCom.wsComponentTag = L"Com_Texture";
	tagLayerCom.bTextOut = true;
	//HUD_Mana_Bar
	pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)SceneID,
		L"Layer_HUD_ManaBar",
		(CGameObject**)&m_pHUD_ManaBar, &tagLayerCom);

#pragma endregion

#pragma region HUD_Mana_Bar
	//HUD_Ammo_Bar
	tagLayerCom.tUIDesc.vUISize.x = 535.f;
	tagLayerCom.tUIDesc.vUISize.y = 90.f;
	tagLayerCom.tUIDesc.vUISize.z = 0.f;
	tagLayerCom.tUIDesc.vUIPos.x = 925.f;
	tagLayerCom.tUIDesc.vUIPos.y = -452.f;
	tagLayerCom.tUIDesc.vUIPos.z = 1.f;
	tagLayerCom.tUIDesc.vCenter = _vector(1.f, 0.f, 0.f);
	tagLayerCom.wsPrototypeTag = L"Component_Texture_AmmoBar";
	tagLayerCom.wsComponentTag = L"Com_Texture";
	tagLayerCom.bTextOut = true;
	//HUD_Mana_Bar
			pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)SceneID,
		L"Layer_HUD_AmmoBar",
		(CGameObject**)&m_pHUD_AmmoBar, &tagLayerCom);
#pragma endregion
#pragma endregion

}

void CUIManager::OnAllUI()
{
	m_pPlayerInfoUI->SetShownBarUI();
	m_pWeaponAmmoInfoUI->SetShownBarUI();
	m_pHUD_HpBar->SetShownBarUI();
	m_pHUD_ManaBar->SetShownBarUI();
	m_pHUD_AmmoBar->SetShownBarUI();
	m_pHUD_TopUI->SetShownBarUI();
}

void CUIManager::OffAllUI()
{
	m_pPlayerInfoUI->SetInvisibleBarUI();
	m_pWeaponAmmoInfoUI->SetInvisibleBarUI();
	m_pHUD_HpBar->SetInvisibleBarUI();
	m_pHUD_ManaBar->SetInvisibleBarUI();
	m_pHUD_AmmoBar->SetInvisibleBarUI();
	m_pHUD_TopUI->SetInvisibleBarUI();
}

void CUIManager::OnPlayerInfo(PLAYER_INFO* _pPlayerDesc)
{
	m_pHUD_HpBar->SetMaxValueAndMinValue(&_pPlayerDesc->iMaxHP, &_pPlayerDesc->iMinHP);
	
	m_pHUD_ManaBar->SetMaxValueAndMinValue(&_pPlayerDesc->iMaxMana, &_pPlayerDesc->iMinMana);
}

void CUIManager::OffPlayerInfo()
{
}

void CUIManager::OnWeaponAmmom(WEAPON_INFO* _pWeaponDesc)
{
	m_pHUD_AmmoBar->SetMaxValueAndMinValue(&_pWeaponDesc->iMaxAmmo, &_pWeaponDesc->iMinAmmo);
}

void CUIManager::OffWeaponAmmom()
{
}

void CUIManager::OnMonsterBar(_int* _iMaxHP, _int* _iMinHP)
{
	m_pHUD_TopUI->SetMaxHPAndHP(_iMaxHP, _iMinHP);
}

void CUIManager::Free()
{
	SafeRelease(m_pDevice);
}
