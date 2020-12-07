#include "stdafx.h"
#include "..\Headers\Loading.h"
//#include "Terrain.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice, ESceneID eNextSceneID)
	: m_pDevice(pDevice)
	, m_eNextSceneID(eNextSceneID)
{
	SafeAddRef(m_pDevice);
}

_bool CLoading::IsFinished() const
{
	return m_IsFinished;
}

HRESULT CLoading::ReadyLoading()
{
	m_hThread = (HANDLE)_beginthreadex(0, 0, ThreadMain, this, 0, nullptr);
	if (nullptr == m_hThread)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSec);

	return S_OK;
}

HRESULT CLoading::ReadyStageResources()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	return S_OK;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pDevice, ESceneID eNextSceneID)
{
	CLoading* pInstance = new CLoading(pDevice, eNextSceneID);
	if (FAILED(pInstance->ReadyLoading()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Loading");
		SafeRelease(pInstance);		
	}

	return pInstance;
}

void CLoading::Free()
{
	SafeRelease(m_pDevice);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSec);
}

unsigned CLoading::ThreadMain(void * pParam)
{
	CLoading* pLoading = (CLoading*)pParam;
	if (nullptr == pLoading)
		return 0;

	EnterCriticalSection(&pLoading->m_CriticalSec);

	HRESULT hr = 0;

	switch (pLoading->m_eNextSceneID)
	{
	case ESceneID::Menu:
		//hr = pLoading->ReadyStageResources();
		break;
	case ESceneID::Stage:
		//hr = pLoading->ReadyStageResources();
		break;
	}

	LeaveCriticalSection(&pLoading->m_CriticalSec);

	if (FAILED(hr))
		return 0;		

	pLoading->m_IsFinished = true;

	return 0;
}
