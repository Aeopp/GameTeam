#include "stdafx.h"
#include "..\Headers\Logo.h"
#include "Stage.h"
#include "Loading.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CLogo::ReadyScene()
{
	CScene::ReadyScene();

	return S_OK;
}

_uint CLogo::UpdateScene()
{
	CScene::UpdateScene();
	return _uint();
}

_uint CLogo::LateUpdateScene()
{
	CScene::LateUpdateScene();

	return _uint();
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CLogo* pInstance = new CLogo(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Logo");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
}
