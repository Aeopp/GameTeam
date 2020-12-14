#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "Player.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "CollisionComponent.h"

#include "BatGrey.h"	// 박쥐

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	SafeAddRef(m_pManagement);
}

HRESULT CMainApp::ReadyMainApp()
{
	if (FAILED(m_pManagement->ReadyEngine(g_hWnd, WINCX, WINCY,
		EDisplayMode::Window, (_uint)ESceneID::MaxCount)))
	{
		PRINT_LOG(L"Error", L"Failed To ReadyEngine");
		return E_FAIL;
	}

	m_pDevice = m_pManagement->GetDevice();
	if (nullptr == m_pDevice)
		return E_FAIL;

	ImGuiHelper::Init(g_hWnd, m_pDevice);

	//SafeAddRef(m_pDevice);	// ImGui Init함수 안에서 이미 증가시키고 있음

	if (FAILED(ReadyStaticResources()))
		return E_FAIL;

	if (FAILED(ReadyDefaultSetting()))
		return E_FAIL;

	if (FAILED(m_pManagement->SetUpCurrentScene((_int)ESceneID::Logo,
		CLogo::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
		return E_FAIL;
	}

	srand(0);	// 랜덤 시드값

	return S_OK;
}

int CMainApp::UpdateMainApp()
{
	ImGuiHelper::UpdateStart();
	m_pManagement->UpdateEngine();

	ImGuiHelper::Text();
	ImGuiHelper::Slider();
	ImGui::Checkbox("Debug ?", &m_pManagement->bDebug);
	ImGui::Checkbox("Imgui Edit On ?", &ImGuiHelper::bEditOn);
	ImGuiHelper::UpdateEnd();
	m_pManagement->RenderEngine();
	ImGuiHelper::Render();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, g_hWnd, nullptr);

	return 0;
}

HRESULT CMainApp::ReadyStaticResources()
{
	/* For.GameObject */
#pragma region GameObject_Player
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlayer>(),
		CPlayer::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	// 박쥐
#pragma region GameObject_BatGrey
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CBatGrey>(),
		CBatGrey::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CMainCamera>(),
		CMainCamera::Create(m_pDevice))))
		return E_FAIL;

	/* For.Component */
#pragma region Component_VIBuffer_RectTexture
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_VIBuffer_RectTexture",
		CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
	
#pragma region Component_Transform
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CTransform>(),
		CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pManagement->AddComponentPrototype(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CCollisionComponent::Create(m_pDevice))))
	{
		return E_FAIL;
	}

#pragma region Component_Texture_Player

#pragma endregion

#pragma region Component_Camera

#pragma endregion
	
	// 박쥐 텍스처들
#pragma region Component_Texture_BatGrey
	// 플라이
#pragma region Component_Texture_BatGreyFly
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyFly",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Fly/batGreyFly%d.png", 8))))
		return E_FAIL;
#pragma endregion
	// 원거리 공격
#pragma region Component_Texture_BatGreyShoot
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyShoot",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Shoot/batGreyShoot%d.png", 5))))
		return E_FAIL;
#pragma endregion
	// 근접 공격
#pragma region Component_Texture_BatGreyAttack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyAttack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Attack/batGreyattack%d.png", 4))))
		return E_FAIL;
#pragma endregion
	// 뒤돌아봄
#pragma region Component_Texture_BatGreyBack
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyBack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Back/batGreyFly_back%d.png", 7))))
		return E_FAIL;
#pragma endregion
	// 피격
#pragma region Component_Texture_BatGreyHit
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyHit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Hit/batGreyHit_%d.png", 2))))
		return E_FAIL;
#pragma endregion
	// 죽음
#pragma region Component_Texture_BatGreyDeath
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyDeath",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BatGrey/Death/batGreydeath%d.png", 11))))
		return E_FAIL;
#pragma endregion

#pragma endregion	// Component_Texture_BatGrey

	return S_OK;
}

HRESULT CMainApp::ReadyDefaultSetting()
{
	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->ReadyMainApp()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMainApp");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pManagement);
	CKeyMgr::Destroy_Instance();
	CManagement::ReleaseEngine();
}
