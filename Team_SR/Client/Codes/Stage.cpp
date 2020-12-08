#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CStage::ReadyScene()
{
	CScene::ReadyScene();

	if (FAILED(AddPlayerLayer(L"Layer_Player")))
		return E_FAIL;

	{
		const std::wstring Type = TYPE_NAME<CMainCamera>();
		const std::wstring GameObjTag = CGameObject::Tag + Type ;
		const std::wstring LayerTag = CLayer::Tag + Type;

		if (FAILED ( m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			GameObjTag,
			(_int)ESceneID::Stage,
			LayerTag,
			reinterpret_cast<CGameObject**>(&_Camera), nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

_uint CStage::UpdateScene(float fDeltaTime)
{
	CScene::UpdateScene(fDeltaTime);

	KeyProcess(fDeltaTime);

	return _uint();
}

_uint CStage::LateUpdateScene()
{
	CScene::LateUpdateScene();

	return _uint();
}

_uint CStage::KeyProcess(float fDeltaTime)
{
	return _uint();
}

HRESULT CStage::AddPlayerLayer(const wstring& LayerTag)
{
	// EXAMPLE
	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		L"GameObject_Player",
		(_int)ESceneID::Stage,
		LayerTag,
		(CGameObject**)&m_pPlayer)))
		return E_FAIL;

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage* pInstance = new CStage(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	PRINT_LOG(L"Test", L"스테이지 생성");

	return pInstance;
}

void CStage::Free()
{
	SafeRelease(m_pPlayer);
	CScene::Free();
}
