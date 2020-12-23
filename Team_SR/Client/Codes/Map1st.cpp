#include "stdafx.h"
#include "Map1st.h"
#include "DXWrapper.h"


CMap1st::CMap1st(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap1st::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld = MATH::WorldMatrix({ 5,5,5 }, { 0,0,0 }, { 0,0,0 });
	LoadMap(L"..\\Resources\\Map\\1\\", MapWorld);
	LoadFloor(L"..\\Resources\\Map\\1\\");
	LoadBars(L"..\\Resources\\Map\\1\\");
	LoadCubeMap(L"..\\Resources\\CubeMap\\Red.dds");

	return S_OK;
}

HRESULT CMap1st::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	
	return S_OK;
}

_uint CMap1st::UpdateGameObject(float fDeltaTime)
{
	return Super::UpdateGameObject(fDeltaTime);
}

_uint CMap1st::LateUpdateGameObject(float fDeltaTime)
{
	

	return Super::LateUpdateGameObject(fDeltaTime);
}

HRESULT CMap1st::RenderGameObject()
{


	return 	Super::RenderGameObject();
}

CMap1st* CMap1st::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap1st* pInstance = new CMap1st(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap1st::Clone(void * pArg)
{
	CMap1st* pClone = new CMap1st(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap1st::Free()
{
	Super::Free();
}
