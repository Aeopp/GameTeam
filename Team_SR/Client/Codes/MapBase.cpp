#include "stdafx.h"
#include "MapBase.h"

CMapBase::CMapBase(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	
}

HRESULT CMapBase::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMapBase::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;
	

	return S_OK;
}

_uint CMapBase::UpdateGameObject(float fDeltaTime)
{
	m_pTransformCom->UpdateTransform();

	return _uint();
}

_uint CMapBase::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CMapBase::RenderGameObject()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	return S_OK;
}



CMapBase* CMapBase::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMapBase* pInstance = new CMapBase(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CTerrain");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMapBase::Clone(void * pArg)
{
	CMapBase* pClone = new CMapBase(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMapBase::Free()
{
	Super::Free();
}
