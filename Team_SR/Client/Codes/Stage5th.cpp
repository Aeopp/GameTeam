#include "stdafx.h"
#include "..\Headers\Stage5th.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlayerInfoUI.h"
#include "Eyebat.h"
#include "Map5th.h"
#include "Stage6th.h"


CStage5th::CStage5th(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{};

HRESULT CStage5th::ReadyScene()
{
	CurrentSceneID = ESceneID::Stage5th;
	NextSceneID = ESceneID::Stage6th;
	using MapType = CMap5th;
	BgmKey = L"006 Medieval - Oubliette.wav";

	Super::ReadyScene();
	CPlayer::InitInfo _InitInfo;
	_InitInfo.SceneID = CurrentSceneID;
	_InitInfo.Location = { 19,5,-28 };

	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlayer>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CPlayer>(),
		(CGameObject**)&m_pPlayer, &_InitInfo)))
		return E_FAIL;

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
		reinterpret_cast<CGameObject**>(&_CurrentMap), &CurrentSceneID)))
		return E_FAIL;

	LoadObjects(L"..\\Resources\\Map\\5\\DecoItemData.obj", vec3{ 2.5,2.5,2.5 });


	return S_OK;
}

_uint CStage5th::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

_uint CStage5th::LateUpdateScene()
{
	return 	Super::LateUpdateScene();
}

_uint CStage5th::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage6th::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	return _uint();
}

void CStage5th::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStage5th* CStage5th::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage5th* pInstance = new CStage5th(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage5th::Free()
{
	Super::Free();
}
