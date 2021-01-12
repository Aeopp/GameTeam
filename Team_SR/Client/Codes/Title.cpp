#include "stdafx.h"
#include "..\Headers\Title.h"
#include "ButtonUI.h"
#include "ImGuiHelper.h"
#include "Stage1st.h"
#include "VIBuffer_UITexture.h"
#include "Texture.h"
#include "GameObject.h"
#include "SceneImage.h"

USING(Engine)
CTitle::CTitle(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CTitle::ReadyScene()
{
	CScene::ReadyScene();

	CurrentSceneID = ESceneID::Menu;
	NextSceneID = ESceneID::Stage1st;

#pragma region Component_Texture_StartButton;
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_StartButton_highlighted",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/Shop/upgrade_next_highlighted.png", 1))))
		return E_FAIL;

	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_StartButton_pressed",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/Shop/upgrade_next_pressed.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SceneTexture;
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_TitleScene",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/SceneImage/Tile_Scene.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Add_GameObject
#pragma region GameObject_WeaponUI
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CSceneImage>(),
		CSceneImage::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	UI_BAR_ADD_COMPONENT tagLayerCom;
#pragma region Add_Layer
#pragma region HUD_HP_Bar
	//HUD_HP_Bar
	tagLayerCom.tUIDesc.vUISize.x = 312.f;
	tagLayerCom.tUIDesc.vUISize.y = 60.f;
	tagLayerCom.tUIDesc.vUISize.z = 0;
	tagLayerCom.tUIDesc.vUIPos.x = -742;
	tagLayerCom.tUIDesc.vUIPos.y = -410;
	tagLayerCom.tUIDesc.vUIPos.z = 2.f;
	tagLayerCom.tUIDesc.vCenter = _vector(-1.f, 0.f, 0.f);
	tagLayerCom.wsPrototypeTag = L"Component_Texture_TitleScene";
	tagLayerCom.wsComponentTag = L"Com_Texture";
	tagLayerCom.bTextOut = true;

	//HUD_HP_Bar
	m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_SceneImage",
		(_int)ESceneID::Menu,
		L"Layer_TitleImage",
		(CGameObject**)&m_pTitleImage, &tagLayerCom);

#pragma endregion

#pragma endregion

	return S_OK;
}

_uint CTitle::UpdateScene(float fDeltaTime)
{
	CScene::UpdateScene(fDeltaTime);
	return  KeyProcess(fDeltaTime);
}

_uint CTitle::LateUpdateScene()
{
	

	return CScene::LateUpdateScene();
}

_uint CTitle::KeyProcess(float fDeltaTime)
{
	if (m_pKeyMgr->Key_Down(VK_RETURN))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)ESceneID::Stage1st,
			CStage1st::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	if (m_pKeyMgr->Key_Down('P'))
	{
		ImGuiHelper::bEditOn = !ImGuiHelper::bEditOn;
	}

	return _uint();
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CTitle* pInstance = new CTitle(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CTitle");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CTitle::Free()
{
	CScene::Free();
}
