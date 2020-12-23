#include "..\Headers\Scene.h"
#include "Management.h"
USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice),m_pKeyMgr(CKeyMgr::Get_Instance()),m_pManagement(CManagement::Get_Instance())
{
	//SafeAddRef(pDevice);
	SafeAddRef(m_pDevice);
}

HRESULT CScene::ReadyScene()
{
	return S_OK;
}

_uint CScene::UpdateScene(float fDeltaTime)
{
	return _uint();
}

_uint CScene::LateUpdateScene()
{
	m_pKeyMgr->Key_Update();
	return _uint();
}

_uint CScene::KeyProcess(float fDeltaTime)
{
	return _uint();
}

void CScene::Free()
{
	SafeRelease(m_pDevice);
}


