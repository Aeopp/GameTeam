#include "stdafx.h"
#include "..\Headers\Stage2nd.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlayerInfoUI.h"
#include "Eyebat.h"
#include "Map2nd.h"
#include "Stage3rd.h"

CStage2nd::CStage2nd(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{}


HRESULT CStage2nd::ReadyScene()
{
	CurrentSceneID = ESceneID::Stage2nd;
	NextSceneID = ESceneID::Stage3rd;
	using MapType = CMap2nd;

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


	LoadObjects(L"..\\Resources\\Map\\2\\GameObjectData.obj", vec3{ 2.5,2.5,2.5 });
	


	return S_OK;
}

_uint CStage2nd::UpdateScene(float fDeltaTime)
{
	return Super::UpdateScene(fDeltaTime);
}

_uint CStage2nd::LateUpdateScene()
{
	return 	Super::LateUpdateScene();
}

_uint CStage2nd::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	if (m_pKeyMgr->Key_Down(VK_SHIFT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		if (FAILED(pManagement->SetUpCurrentScene((_int)NextSceneID,
			CStage3rd::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed To SetUpCurrentScene");
			return 0;
		}

		return CHANGE_SCNENE;
	}

	return _uint();
}

void CStage2nd::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);
}

CStage2nd* CStage2nd::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage2nd* pInstance = new CStage2nd(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage2nd::Free()
{
	Super::Free();
}
