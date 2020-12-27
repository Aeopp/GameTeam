#include "stdafx.h"
#include "Map5th.h"
#include "DXWrapper.h"


CMap5th::CMap5th(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap5th::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 2.5,2.5,2.5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\5\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\5\\");
	LoadBars(L"..\\Resources\\Map\\5\\"); 
	LoadCubeMap(L"..\\Resources\\CubeMap\\Blue.dds");

	return S_OK;
}

HRESULT CMap5th::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMap5th::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap5th::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap5th::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap5th* CMap5th::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap5th* pInstance = new CMap5th(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap5th::Clone(void * pArg)
{
	CMap5th* pClone = new CMap5th(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap5th::Free()
{
	Super::Free();
}
