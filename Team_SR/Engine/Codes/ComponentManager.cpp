#include "..\Headers\ComponentManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponentManager)

CComponentManager::CComponentManager()
{
}

HRESULT CComponentManager::ReserveSizePrototypeContainer(_int iSceneCount)
{
	if (0 > iSceneCount)
		return E_FAIL;

	m_iSceneCount = iSceneCount;
	m_pPrototypes = new PROTOTYPES[m_iSceneCount];
	if (nullptr == m_pPrototypes)
	{
		PRINT_LOG(L"Error", L"Failed To ReserveSizePrototypeContainer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CComponentManager::AddComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	CComponent * pPrototype)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return E_FAIL;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(ComponentTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		m_pPrototypes[iSceneIndex].insert(make_pair(ComponentTag, pPrototype));
	}

	return S_OK;
}

CComponent* CComponentManager::CloneComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	void* pArg)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return nullptr;
	}

	auto iter_find = m_pPrototypes[iSceneIndex].find(ComponentTag);
	if (m_pPrototypes[iSceneIndex].end() == iter_find)
	{
		TCHAR szBuffer[128] = L"";
		swprintf_s(szBuffer, L"Not found %s Prototype", ComponentTag.c_str());

		PRINT_LOG(L"Warning", szBuffer);
		return nullptr;
	}

	return iter_find->second->Clone(pArg);
}

HRESULT CComponentManager::ClearForScene(_int iSceneIndex)
{
	if (0 > iSceneIndex ||
		m_iSceneCount <= iSceneIndex)
	{
		PRINT_LOG(L"Error", L"Out of range Component PrototypeContainer");
		return E_FAIL;
	}

	for (auto& Pair : m_pPrototypes[iSceneIndex])
	{
		SafeRelease(Pair.second);
	}
	m_pPrototypes[iSceneIndex].clear();

	return S_OK;
}

void CComponentManager::Free()
{
	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			SafeRelease(Pair.second);
		}
		m_pPrototypes[i].clear();
	}

	SafeDeleteArray(m_pPrototypes);	
}
