#include "stdafx.h"
#include "..\Headers\Logo.h"
#include "Stage.h"
#include "Loading.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CLogo::ReadyScene()
{
	CScene::ReadyScene();

	m_pLoading = CLoading::Create(m_pDevice, ESceneID::Stage);
	if (nullptr == m_pLoading)
		return E_FAIL;

	return S_OK;
}

_uint CLogo::UpdateScene()
{
	CScene::UpdateScene();

	if (GetAsyncKeyState(VK_RETURN) & 0x8000 &&
		true == m_pLoading->IsFinished())
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)ESceneID::Stage,
			CStage::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}		

		return CHANGE_SCNENE;
	}

	return _uint();
}

_uint CLogo::LateUpdateScene()
{
	CScene::LateUpdateScene();

	return _uint();
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CLogo* pInstance = new CLogo(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Logo");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
	SafeRelease(m_pLoading);
}
