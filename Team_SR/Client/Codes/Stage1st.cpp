#include "stdafx.h"
#include "..\Headers\Stage1st.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlyerInfoUI.h"
#include "Eyebat.h"
#include "Stage2nd.h"
#include "Map2nd.h"

CStage1st::CStage1st(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{};

HRESULT CStage1st::ReadyScene()
{
	CurrentSceneID = ESceneID::Stage1st;
	NextSceneID = ESceneID::Stage2nd;
	using MapType = CMap1st;

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

	SpawnObjectFromName(L"Torch", { -10.f, 10.f, 10.f });
	SpawnObjectFromName(L"Candle", { -9.f, 10.f, 10.f });
	SpawnObjectFromName(L"Barrel", { -7.f, 10.f, 10.f });
	SpawnObjectFromName(L"BarrelBomb", { -5.f, 10.f, 10.f });
	SpawnObjectFromName(L"BarrelWaste", { -3.f, 10.f, 10.f });
	SpawnObjectFromName(L"Box1", { -10.f, 10.f, 14.f });
	SpawnObjectFromName(L"Box2", { -8.f, 10.f, 14.f });
	SpawnObjectFromName(L"BoxSteel1", { -6.f, 10.f, 14.f });
	SpawnObjectFromName(L"BoxSteel2", { -4.f, 10.f, 14.f });
	SpawnObjectFromName(L"Headstone1", { 0.f, 10.f, 10.f });
	SpawnObjectFromName(L"Headstone2", { 3.f, 10.f, 10.f });
	SpawnObjectFromName(L"ThornyVine", { 0.f, 10.f, 14.f });
	SpawnObjectFromName(L"TreeBlight", { 3.f, 10.f, 14.f });
	SpawnObjectFromName(L"Tree1", { 6.f, 10.f, 14.f });
	SpawnObjectFromName(L"Tree2", { 9.f, 10.f, 14.f });
	SpawnObjectFromName(L"Dead_Caleb", { -9.f, 10.f, 16.f });
	SpawnObjectFromName(L"Dead_Doomguy", { -6.f, 10.f, 16.f });
	SpawnObjectFromName(L"Dead_Duke", { -3.f, 10.f, 16.f });
	SpawnObjectFromName(L"Dead_Sam", { 0.f, 10.f, 16.f });
	SpawnObjectFromName(L"Dead_Wang", { 3.f, 10.f, 16.f });

	
	return S_OK;
}

_uint CStage1st::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime); 
}

_uint CStage1st::LateUpdateScene()
{
	return Super::LateUpdateScene();
}

_uint CStage1st::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;
		
		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage2nd::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	return _uint();
}

void CStage1st::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStage1st* CStage1st::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage1st* pInstance = new CStage1st(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage1st::Free()
{
	Super::Free();
}
