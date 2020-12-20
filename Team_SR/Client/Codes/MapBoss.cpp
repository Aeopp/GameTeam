#include "stdafx.h"
#include "MapBoss.h"
#include "DXWrapper.h"


CMapBoss::CMapBoss(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMapBoss::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 5,5,5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\Boss\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\Boss\\");
	LoadBars(L"..\\Resources\\Map\\Boss\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\Red.dds");

	return S_OK;
}

HRESULT CMapBoss::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMapBoss::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMapBoss::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMapBoss::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMapBoss* CMapBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMapBoss* pInstance = new CMapBoss(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMapBoss::Clone(void * pArg)
{
	CMapBoss* pClone = new CMapBoss(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMapBoss::Free()
{
	Super::Free();
}
