#include "stdafx.h"
#include "..\Headers\Stage1st.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
CStage1st::CStage1st(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{
}

HRESULT CStage1st::ReadyScene()
{
	Super::ReadyScene();

	{
		const wstring GameObjTag = CGameObject::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Stage1st,
			GameObjTag,
			CMap1st::Create(m_pDevice))))
			return E_FAIL;

		const std::wstring LayerTag = CLayer::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Stage1st,
			GameObjTag,
			(_int)ESceneID::Stage1st,
			LayerTag,
			reinterpret_cast<CGameObject**>(&_CurrentMap), nullptr)))
			return E_FAIL;
		// 박쥐
		MonsterBasicArgument stArg;
		stArg.uiSize = sizeof(MonsterBasicArgument);
		stArg.pPlayer = m_pPlayer;
		stArg.vPosition = { 0.f, 5.f, 30.f };
		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CBatGrey>(),
			(_int)ESceneID::Stage1st,
			CLayer::Tag + TYPE_NAME<CBatGrey>(),
			nullptr, static_cast<void*>(&stArg))))
			return E_FAIL;

		// 글레이서
		stArg.vPosition = { 5.f, 10.f, 30.f };

		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CGlacier>(),
			(_int)ESceneID::Stage1st,
			CLayer::Tag + TYPE_NAME<CGlacier>(),
			nullptr, static_cast<void*>(&stArg))))
			return E_FAIL;
	}


	return S_OK;
}

_uint CStage1st::UpdateScene(float fDeltaTime)
{
	Super::UpdateScene(fDeltaTime);

	return _uint();
}

_uint CStage1st::LateUpdateScene()
{
	Super::LateUpdateScene();

	return _uint();
}

_uint CStage1st::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

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

	PRINT_LOG(L"Test", L"스테이지 1 생성");

	return pInstance;
}

void CStage1st::Free()
{
	Super::Free();
}
