#include "stdafx.h"
#include "..\Headers\Logo.h"
#include "Loading.h"
#include "ImGuiHelper.h"
#include "Stage2nd.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CLogo::ReadyScene()
{
	CScene::ReadyScene();
	
	return S_OK;
}

_uint CLogo::UpdateScene(float fDeltaTime)
{
	CScene::UpdateScene(fDeltaTime);
	return  KeyProcess(fDeltaTime);

} 

_uint CLogo::LateUpdateScene()
{
	return CScene::LateUpdateScene();
}

_uint CLogo::KeyProcess(float fDeltaTime)
{
	if (m_pKeyMgr->Key_Down(VK_RETURN))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)ESceneID::Stage2nd,
			CStage2nd::Create(m_pDevice))))
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
}
