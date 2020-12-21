#include "stdafx.h"
#include "Map6th.h"
#include "DXWrapper.h"


CMap6th::CMap6th(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap6th::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 5,5,5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\6\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\6\\");
	LoadBars(L"..\\Resources\\Map\\6\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\LightBlue.dds");

	return S_OK;
}

HRESULT CMap6th::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMap6th::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap6th::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap6th::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap6th* CMap6th::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap6th* pInstance = new CMap6th(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap6th::Clone(void * pArg)
{
	CMap6th* pClone = new CMap6th(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap6th::Free()
{
	Super::Free();
}
