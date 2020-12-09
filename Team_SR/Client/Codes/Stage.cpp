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
		if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CPlayer>(),
			(_int)ESceneID::Stage1st,
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
			(_int)ESceneID::Stage1st,
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

void CStage::Free()
{
	SafeRelease(m_pPlayer);
	SafeRelease(_Camera);
	SafeRelease(_CurrentMap);

	CScene::Free();
}
