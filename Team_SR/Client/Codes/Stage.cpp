#include "stdafx.h"
#include "..\Headers\Stage.h"


CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CStage::ReadyScene()
{
	CScene::ReadyScene();	

	if (FAILED(AddTerrainLayer(L"Layer_Terrain")))
		return E_FAIL;

	if (FAILED(AddPlayerLayer(L"Layer_Player")))
		return E_FAIL;


	return S_OK;
}

_uint CStage::UpdateScene()
{
	CScene::UpdateScene();

	return _uint();
}

_uint CStage::LateUpdateScene()
{
	CScene::LateUpdateScene();

	return _uint();
}

HRESULT CStage::AddPlayerLayer(const wstring & LayerTag)
{
	return S_OK;
}

HRESULT CStage::AddTerrainLayer(const wstring & LayerTag)
{
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage* pInstance = new CStage(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}
