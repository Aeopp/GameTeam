#include "..\Headers\GameObject.h"
#include "Management.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject & other)
	: m_pDevice(other.m_pDevice)
{
	SafeAddRef(m_pDevice);
}

CComponent* CGameObject::GetComponent(const wstring & ComponentTag)
{
	auto iter_find = m_Components.find(ComponentTag);
	if(m_Components.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

HRESULT CGameObject::ReadyGameObjectPrototype()
{
	return S_OK;
}

HRESULT CGameObject::ReadyGameObject(void * pArg)
{
	return S_OK;
}

_uint CGameObject::UpdateGameObject(float fDeltaTime)
{
	return _uint();
}

_uint CGameObject::LateUpdateGameObject(float fDeltaTime)
{
	return _uint();
}

HRESULT CGameObject::RenderGameObject()
{
	return S_OK;
}

void CGameObject::Free()
{
	SafeRelease(m_pDevice);

	for (auto& Pair : m_Components)
	{
		SafeRelease(Pair.second);
	}

	m_Components.clear();
}

HRESULT CGameObject::AddComponent(
	_int iSceneIndex, 
	const wstring& PrototypeTag,
	const wstring& ComponentTag, 
	CComponent** ppComponent, 
	void * pArg)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CComponent* pClone = pManagement->CloneComponentPrototype(iSceneIndex, PrototypeTag, pArg);
	if (nullptr == pClone)
		return E_FAIL;

	m_Components.insert(make_pair(ComponentTag, pClone));

	if (ppComponent)
	{
		*ppComponent = pClone;
		SafeAddRef(pClone);
	}

	return S_OK;
}
