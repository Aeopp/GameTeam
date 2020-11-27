#include "Graphic_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_pSDK(nullptr)
	, m_pDevice(nullptr)
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, EDisplayMode eDisplayMode)
{
	D3DCAPS9 DeviceCaps; 
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9)); 

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		PRINT_LOG(L"Error", L"GetDevice Caps Failed");
		return E_FAIL;
	}
	/*
	여기에서 한개를 더 조사를 해야하는데 버텍스 프로세싱이라는걸 하드웨어에서 할수 있는지를 조사를 해야한다. 
	정점변환 + 조명연산 = 버텍스 프로세싱 

	*/
	DWORD vp = 0; 
	if (DeviceCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING ;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; 

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality= 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = (BOOL)eDisplayMode;// TRUE일 경우 창모드, FALSE 전체화면. 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		PRINT_LOG(L"Error", L"GraphicDevice Creating Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Free()
{
	if (SafeRelease(m_pDevice))
	{
		PRINT_LOG(L"Warning", L"Failed To Releasing m_pDevice");
	}

	if(SafeRelease(m_pSDK))
	{
		PRINT_LOG(L"Warning", L"Failed To Releasing m_pSDK");
	}
}
