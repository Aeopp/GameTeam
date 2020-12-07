#include "..\Headers\GameObject.h"
#include "Management.h"



USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice),m_pManagement(CManagement::Get_Instance())
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
	AddStaticComponents();

	if (pArg)
	{
		_vector vPosition;
		memcpy(&vPosition, pArg, sizeof(_vector));

		m_pTransformCom->m_TransformDesc.vPosition = vPosition;
	}

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

	CComponent* pClone = m_pManagement->CloneComponentPrototype(iSceneIndex, PrototypeTag, pArg);
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

HRESULT CGameObject::AddStaticComponents()
{	
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */

	if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_Transform",
		L"Com_Transform",
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}
