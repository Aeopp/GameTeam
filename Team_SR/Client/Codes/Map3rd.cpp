#include "stdafx.h"
#include "Map3rd.h"
#include "DXWrapper.h"


CMap3rd::CMap3rd(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap3rd::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 2.5,2.5,2.5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\3\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\3\\");
	LoadBars(L"..\\Resources\\Map\\3\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\LightBlue.dds");

	return S_OK;
}

HRESULT CMap3rd::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMap3rd::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap3rd::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap3rd::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap3rd* CMap3rd::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap3rd* pInstance = new CMap3rd(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap3rd::Clone(void * pArg)
{
	CMap3rd* pClone = new CMap3rd(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap3rd::Free()
{
	Super::Free();
}
