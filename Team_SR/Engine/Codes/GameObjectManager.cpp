#include "..\Headers\GameObjectManager.h"
#include "Layer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGameObjectManager)

CGameObjectManager::CGameObjectManager()
{
}

CGameObject * CGameObjectManager::GetGameObject(_int iSceneIndex, const wstring & LayerTag, _uint iIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
		return nullptr;

	auto iter_find = m_pLayers[iSceneIndex].find(LayerTag);
	if (m_pLayers[iSceneIndex].end() == iter_find)
		return nullptr;

	return iter_find->second->GetGameObject(iIndex);
}

HRESULT CGameObjectManager::ReserveSizeContainer(_int iSceneCount)
{
	if (0 > iSceneCount)
		return E_FAIL;

	m_iSceneCount = iSceneCount;
	m_pPrototypes = new PROTOTYPES[m_iSceneCount];
	if (nullptr == m_pPrototypes)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizeContainer");
		return E_FAIL;
	}

	m_pLayers = new LAYERS[m_iSceneCount];
	if (nullptr == m_pLayers)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizeContainer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameObjectManager::AddGameObjectPrototype(
	_int iSceneIndex, 
	const wstring & GameObjectTag, 
	CGameObject * pPrototype)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return E_FAIL;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		m_pPrototypes[iSceneIndex].insert(make_pair(GameObjectTag, pPrototype));
	}

	return S_OK;
}

HRESULT CGameObjectManager::AddGameObjectInLayer(
	_int iFromSceneIndex, 
	const wstring& GameObjectTag, 
	_int iToSceneIndex, 
	const wstring& LayerTag, 
	CGameObject** ppGameObject,
	void* pArg)
{
	if (0 > iFromSceneIndex ||
		m_iSceneCount <= iFromSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObjectMagager Containers");
		return E_FAIL;
	}

	if (0 > iToSceneIndex ||
		m_iSceneCount <= iToSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObjectMagager Containers");
		return E_FAIL;
	}

	auto iter_find = m_pPrototypes[iFromSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iFromSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}

	auto pClone = iter_find->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Failed To Clone %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}
	if (ppGameObject)
	{
		*ppGameObject = pClone;
		SafeAddRef(pClone);
	}

	auto iter_find_Layer = m_pLayers[iToSceneIndex].find(LayerTag);
	if (m_pLayers[iToSceneIndex].end() == iter_find_Layer)
	{
		CLayer* pLayer = CLayer::Create();
		m_pLayers[iToSceneIndex].insert(make_pair(LayerTag, pLayer));
	}

	if (FAILED(m_pLayers[iToSceneIndex][LayerTag]->AddGameObjectInLayer(pClone)))
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Failed To AddGameObjectInLayer %s Clone", LayerTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameObjectManager::ClearForScene(_int iSceneIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return E_FAIL;
	}

	for (auto& Pair : m_pLayers[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}

	m_pLayers[iSceneIndex].clear();

	for (auto& Pair : m_pPrototypes[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}

	m_pPrototypes[iSceneIndex].clear();

	return S_OK;
}

_uint CGameObjectManager::UpdateGameObject(float fDeltaTime)
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->UpdateGameObject(fDeltaTime);
		}
	}

	return _uint();
}

_uint CGameObjectManager::LateUpdateGameObject(float fDeltaTime)
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateUpdateGameObject(fDeltaTime);
		}
	}

	return _uint();
}

// 2020.12.16 11:50 KMJ
// Remove 플래그가 ON된 게임 오브젝트 삭제
void CGameObjectManager::RemoveGameObject()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->RemoveGameObject();
		}
	}
}

CGameObject * CGameObjectManager::CloneGameObjectPrototype(
	_int iSceneIndex,
	const wstring & GameObjectTag,
	void * pArg)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range GameObject PrototypeContainer");
		return nullptr;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(GameObjectTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", GameObjectTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return nullptr;
	}

	return iter_find->second->Clone(pArg);
}

void CGameObjectManager::Free()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			SafeRelease(Pair.second);
		}
		m_pLayers[i].clear();
	}

	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			SafeRelease(Pair.second);
		}
		m_pPrototypes[i].clear();
	}

	SafeDeleteArray(m_pLayers);
	SafeDeleteArray(m_pPrototypes);
}
