#include "stdafx.h"
#include "..\Headers\StageMidBoss.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlayerInfoUI.h"
#include "Eyebat.h"
#include "MapMidBoss.h"
#include "Stage5th.h"


CStageMidBoss::CStageMidBoss(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{};

HRESULT CStageMidBoss::ReadyScene()
{
	CurrentSceneID = ESceneID::StageMidBoss;
	NextSceneID = ESceneID::Stage5th;
	using MapType = CMapMidBoss;
	BgmKey = L"037 Egyptian - Osiris.wav";

	Super::ReadyScene();

	CPlayer::InitInfo _InitInfo;
	_InitInfo.SceneID = CurrentSceneID;
	_InitInfo.Location = { 7,7,2 };

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
	
	LoadObjects(L"..\\Resources\\Map\\MidBoss\\DecoItemData.obj", vec3{ 2.5,2.5,2.5 });


	return S_OK;
}

_uint CStageMidBoss::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

_uint CStageMidBoss::LateUpdateScene()
{
	return 	Super::LateUpdateScene();
}

_uint CStageMidBoss::KeyProcess(float fDeltaTime)
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

void CStageMidBoss::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStageMidBoss* CStageMidBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStageMidBoss* pInstance = new CStageMidBoss(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStageMidBoss::Free()
{
	Super::Free();
}
