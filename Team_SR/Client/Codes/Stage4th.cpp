#include "stdafx.h"
#include "..\Headers\Stage4th.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlyerInfoUI.h"
#include "Eyebat.h"
#include "Map4th.h"
#include "Stage5th.h"


CStage4th::CStage4th(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{};

HRESULT CStage4th::ReadyScene()
{
	CurrentSceneID = ESceneID::Stage4th;
	NextSceneID = ESceneID::Stage5th;
	using MapType = CMap4th;

	Super::ReadyScene();

	const wstring GameObjTag = CGameObject::Tag + TYPE_NAME<MapType>();
	if (FAILED(m_pManagement->AddGameObjectPrototype(
		(_int)CurrentSceneID,
		GameObjTag,
		MapType::Create(m_pDevice))))
		return E_FAIL;

	const std::wstring LayerTag = CLayer::Tag + TYPE_NAME<MapType>();
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)CurrentSceneID,
		GameObjTag,
		(_int)CurrentSceneID,
		LayerTag,
		reinterpret_cast<CGameObject**>(&_CurrentMap), nullptr)))
		return E_FAIL;

	MonsterBasicArgument stArg;
	stArg.uiSize = sizeof(MonsterBasicArgument);
	stArg.pPlayer = m_pPlayer;
	stArg.vPosition = { 0.f, 10.f, 20.f };
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + L"Shark",
		(_int)CurrentSceneID,
		CLayer::Tag + L"Monster",
		nullptr, static_cast<void*>(&stArg))))
		return E_FAIL;

	return S_OK;
}

_uint CStage4th::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

_uint CStage4th::LateUpdateScene()
{
	return 	Super::LateUpdateScene();
}

_uint CStage4th::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage5th::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	return _uint();
}

void CStage4th::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStage4th* CStage4th::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage4th* pInstance = new CStage4th(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage4th::Free()
{
	Super::Free();
}
