#include "stdafx.h"
#include "MainApp.h"
#include "Graphic_Device.h"

// Init

CMainApp::CMainApp()
{
}

HRESULT CMainApp::ReadyMainApp()
{
	return S_OK;
}

int CMainApp::UpdateMainApp()
{
	return 0;
}

void CMainApp::Render()
{
	CGraphic_Device::Get_Instance()->Render_Begin();

	CGraphic_Device::Get_Instance()->Render_End();
}

CMainApp* CMainApp::Create()
{
	CGraphic_Device::Get_Instance()->Ready_Graphic_Device(g_hWnd,
		WINCX, WINCY, EDisplayMode::Window);

	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->ReadyMainApp()))
	{
		//log
		//messagebox

		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
}
