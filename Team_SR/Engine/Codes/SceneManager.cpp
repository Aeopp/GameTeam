#include "..\Headers\SceneManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}

HRESULT CSceneManager::SetUpCurrentScene(_int iSceneID, CScene * pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (m_iSceneID != iSceneID)
	{
		SafeRelease(m_pCurrentScene);
		m_pCurrentScene = pCurrentScene;

		m_iSceneID = iSceneID;
	}

	return S_OK;
}

_uint CSceneManager::UpdateScene()
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->UpdateScene();
}

_uint CSceneManager::LateUpdateScene()
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->LateUpdateScene();
}

void CSceneManager::Free()
{
	SafeRelease(m_pCurrentScene);
}
