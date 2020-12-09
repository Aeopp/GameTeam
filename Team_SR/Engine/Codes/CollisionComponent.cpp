#include "CollisionComponent.h"

USING(Engine)

CCollisionComponent::CCollisionComponent(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{

}

HRESULT CCollisionComponent::ReadyComponentPrototype()
{
	if (FAILED( Super::ReadyComponent()) )
		return E_FAIL;
	

	return S_OK;
}

HRESULT CCollisionComponent::ReadyComponent(void* pArg)
{
	if (FAILED(Super::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}


CCollisionComponent* CCollisionComponent::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollisionComponent* pInstance = new CCollisionComponent(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CCollisionComponent");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent* CCollisionComponent::Clone(void* pArg)
{
	CCollisionComponent* pClone = new CCollisionComponent(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectTexture");
		SafeRelease(pClone);
	}

	return pClone;
}

void CCollisionComponent::Free()
{
	Super::Free();
}
