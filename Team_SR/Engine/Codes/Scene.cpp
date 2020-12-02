#include "..\Headers\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(pDevice);
}

HRESULT CScene::ReadyScene()
{
	return S_OK;
}

_uint CScene::UpdateScene()
{
	return _uint();
}

_uint CScene::LateUpdateScene()
{
	return _uint();
}

void CScene::Free()
{
	SafeRelease(m_pDevice);
}


