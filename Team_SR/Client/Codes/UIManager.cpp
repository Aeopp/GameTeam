#include "stdafx.h"
#include "..\Headers\UIManager.h"
#include "Management.h"
#include "VIBuffer_UITexture.h"
#include "LoadingBar.h"
IMPLEMENT_SINGLETON(CUIManager)

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
#pragma region GameObject_HUD_HpBar
	if (FAILED(pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CLoadingBar>(),
		CLoadingBar::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

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
	
	//Buffer
#pragma region Component_VIBuffer_UITexture
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_UITexture>(),
		CVIBuffer_UITexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
#pragma endregion


	//Add Layer
#pragma region Add_Layer
	//플레이어 정보 - 체력, 마나, 초상화
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

	UI_ADD_COMPONENT tagLayerCom;
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

	//HUD_HP_Bar
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)ESceneID::Stage1st,
		L"Layer_HUD_HpBar",
		(CGameObject**)&m_pHUD_HpBar, &tagLayerCom)))
		return E_FAIL;

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
	//HUD_Mana_Bar
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)ESceneID::Stage1st,
		L"Layer_HUD_ManaBar",
		(CGameObject**)&m_pHUD_ManaBar, &tagLayerCom)))
		return E_FAIL;

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
	//HUD_Mana_Bar
	if (FAILED(pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_LoadingBar",
		(_int)ESceneID::Stage1st,
		L"Layer_HUD_AmmoBar",
		(CGameObject**)&m_pHUD_AmmoBar, &tagLayerCom)))
		return E_FAIL;
#pragma endregion

#pragma region Test_Font

	m_iMax[0] = 100;
	m_iMax[1] = 200;
	m_iMax[2] = 50;

	m_iMin[0] = 100;
	m_iMin[1] = 100;
	m_iMin[2] = 100;

	m_pHUD_HpBar->SetMaxHPAndHP(&m_iMax[0], &m_iMin[0]);
	m_pHUD_ManaBar->SetMaxHPAndHP(&m_iMax[1], &m_iMin[1]);
	m_pHUD_AmmoBar->SetMaxHPAndHP(&m_iMax[2], &m_iMin[2]);
#pragma endregion
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
	SafeRelease(m_pDevice);
}
