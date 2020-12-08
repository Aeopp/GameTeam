#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CStage::ReadyScene()
{
	CScene::ReadyScene();

	{
		const wstring GameObjTag = CGameObject::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Stage,
			GameObjTag,
			CMap1st::Create(m_pDevice))))
			return E_FAIL;

		const std::wstring LayerTag = CLayer::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Stage,
			GameObjTag,
			(_int)ESceneID::Stage,
			LayerTag,
			reinterpret_cast<CGameObject**>(&_CurrentMap), nullptr)))
			return E_FAIL;
	}

	{
		if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CPlayer>(),
			(_int)ESceneID::Stage,
			CLayer::Tag + TYPE_NAME<CPlayer>(),
			(CGameObject**)&m_pPlayer)))
			return E_FAIL;
	}

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

	//{
	//	const std::wstring Type = TYPE_NAME<CMap1st>();
	//	const std::wstring GameObjTag = CGameObject::Tag + Type;
	//	const std::wstring LayerTag = CLayer::Tag + Type;

	//	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Stage,
	//		GameObjTag,
	//		(_int)ESceneID::Stage,
	//		LayerTag,
	//		reinterpret_cast<CGameObject**>(&_CurrentMap), nullptr)))
	//	{
	//		return E_FAIL;
	//	}
	//}

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
	SafeRelease(_Camera);
	SafeRelease(_CurrentMap);

	CScene::Free();
}
