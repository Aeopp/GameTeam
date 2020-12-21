#include "stdafx.h"
#include "Map2nd.h"
#include "DXWrapper.h"


CMap2nd::CMap2nd(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap2nd::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld  = MATH::WorldMatrix({ 5,5,5}, { 0,0,0}, { 0,0,0});
	LoadMap(L"..\\Resources\\Map\\2\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\2\\");
	LoadBars(L"..\\Resources\\Map\\2\\"); 
	
	LoadCubeMap(L"..\\Resources\\CubeMap\\Blue.dds");

	return S_OK;
}

HRESULT CMap2nd::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;


	return S_OK;
}

_uint CMap2nd::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap2nd::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap2nd::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap2nd* CMap2nd::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap2nd* pInstance = new CMap2nd(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap2nd::Clone(void * pArg)
{
	CMap2nd* pClone = new CMap2nd(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap2nd::Free()
{
	Super::Free();
}
