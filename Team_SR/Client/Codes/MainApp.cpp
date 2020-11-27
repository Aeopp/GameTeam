#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "Player.h"

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

	SafeAddRef(m_pDevice);

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

	return S_OK;
}

int CMainApp::UpdateMainApp()
{
	m_pManagement->UpdateEngine();
	m_pManagement->RenderEngine();

	return 0;
}

HRESULT CMainApp::ReadyStaticResources()
{
	/* For.GameObject */
#pragma region GameObject_Player
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)ESceneID::Static,
		L"GameObject_Player",
		CPlayer::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	/* For.Component */
#pragma region Component_VIBuffer_RectColor
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_VIBuffer_RectColor",
		CVIBuffer_RectColor::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_RectTexture
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_VIBuffer_RectTexture",
		CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Player
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_Player",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/태후니%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Transform
	if (FAILED(m_pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Transform",
		CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::ReadyDefaultSetting()
{
	/* 조명 off */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
		return E_FAIL;

	/* 
	D3DCULL_CW(ClockWise): 시계방향인 정점들을 추려낸다.
	D3DCULL_CCW(Counter ClockWise): 반시계방향인 정점들을 추려낸다. 디폴트.
	D3DCULL_NONE: 후면 추려내기 off
	*/
	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	/*
	D3DFILL_WIREFRAME: 와이어 프레임으로 폴리곤을 표현
	D3DFILL_SOLID: 디폴트
	*/
	/*if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)))
		return E_FAIL;*/

	D3DXMATRIX matView, matProj;

	/* 뷰행렬 만드는 함수 */
	D3DXMatrixLookAtLH(
		&matView, /* 반환 값 */
		&D3DXVECTOR3(0.f, 5.f, -5.f), /* 월드상 카메라 위치 Eye */
		&D3DXVECTOR3(0.f, 0.f, 0.f), /* 카메라가 바라보는 지점 At */
		&D3DXVECTOR3(0.f, 1.f, 0.f) /* Up벡터 */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matView)))
		return E_FAIL;

	/* 원근 투영 행렬을 만드는 함수 */
	D3DXMatrixPerspectiveFovLH(
		&matProj, /* 반환 값 */
		D3DXToRadian(90.f), /* 시야각 FovY */
		float(WINCX) / WINCY, /* 종횡비 Aspect */
		1.f, /* 카메라와 근편명과의 Z거리 Near */
		1000.f /* 카메라와 원평면과의 Z거리 Far */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj)))
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
	CManagement::ReleaseEngine();
}
