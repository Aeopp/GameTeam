#include "stdafx.h"
#include "Map4th.h"
#include "DXWrapper.h"


CMap4th::CMap4th(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap4th::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 5,5,5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\4\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\4\\");
	LoadBars(L"..\\Resources\\Map\\4\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\Red.dds");

	return S_OK;
}

HRESULT CMap4th::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMap4th::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap4th::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap4th::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap4th* CMap4th::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap4th* pInstance = new CMap4th(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap4th::Clone(void * pArg)
{
	CMap4th* pClone = new CMap4th(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap4th::Free()
{
	Super::Free();
}
