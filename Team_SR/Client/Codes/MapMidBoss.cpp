#include "stdafx.h"
#include "MapMidBoss.h"
#include "DXWrapper.h"


CMapMidBoss::CMapMidBoss(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMapMidBoss::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld  = MATH::WorldMatrix({ 3,3,3}, { 0,0,0}, { 0,0,0});
	LoadMap(L"..\\Resources\\Map\\MidBoss\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\MidBoss\\");
	LoadBars(L"..\\Resources\\Map\\MidBoss\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\Blue.dds");

	return S_OK;
}

HRESULT CMapMidBoss::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMapMidBoss::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMapMidBoss::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMapMidBoss::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMapMidBoss* CMapMidBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMapMidBoss* pInstance = new CMapMidBoss(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMapMidBoss::Clone(void * pArg)
{
	CMapMidBoss* pClone = new CMapMidBoss(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMapMidBoss::Free()
{
	Super::Free();
}
