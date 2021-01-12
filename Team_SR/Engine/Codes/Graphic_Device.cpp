#include "Graphic_Device.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_pSDK(nullptr)
	, m_pDevice(nullptr)
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, EDisplayMode eDisplayMode )
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

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//d3dpp.MultiSampleType= D3DMULTISAMPLE_16_SAMPLES;
	//d3dpp.MultiSampleQuality = 8;


	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = (BOOL)eDisplayMode;// TRUE일 경우 창모드, FALSE 전체화면. 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	DWORD MsaaSamples;

	ChooseBestMSAAMode(d3dpp.BackBufferFormat,
		d3dpp.AutoDepthStencilFormat,
		d3dpp.Windowed, d3dpp.MultiSampleType,
		d3dpp.MultiSampleQuality,
		MsaaSamples);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, vp, &d3dpp, &m_pDevice)))
	{
		PRINT_LOG(L"Error", L"GraphicDevice Creating Failed");
		return E_FAIL;
	}
	else
	{
		this->D3Dpp = d3dpp;
	}

	D3DXCreateLine(m_pDevice, &_Line);
	/*_Line->SetAntialias(true);
	_Line->SetGLLines(false);
	_Line->SetWidth(10.f);
	_Line->SetPattern(1);*/
	
	return S_OK;
}

void CGraphic_Device::Free()
{

	if (SafeRelease(m_pDevice))
	{
		// 2021.01.08 KMJ
		// 가립시다...
		//PRINT_LOG(L"Warning", L"Failed To Releasing m_pDevice");
	}

	if(SafeRelease(m_pSDK))
	{
		// 2021.01.08 KMJ
		// 가립시다...
		//PRINT_LOG(L"Warning", L"Failed To Releasing m_pSDK");
	}
}

bool CGraphic_Device::MSAAModeSupported(D3DMULTISAMPLE_TYPE type, D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt, BOOL windowed, DWORD& qualityLevels)
{
	DWORD backBufferQualityLevels = 0;
	DWORD depthStencilQualityLevels = 0;

	HRESULT hr = m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, backBufferFmt, windowed, type,
		&backBufferQualityLevels);

	if (SUCCEEDED(hr))
	{
		hr = m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, depthStencilFmt, windowed, type,
			&depthStencilQualityLevels);

		if (SUCCEEDED(hr))
		{
			if (backBufferQualityLevels == depthStencilQualityLevels)
			{
				// The valid range is between zero and one less than the level
				// returned by IDirect3D9::CheckDeviceMultiSampleType().

				if (backBufferQualityLevels > 0)
					qualityLevels = backBufferQualityLevels - 1;
				else
					qualityLevels = backBufferQualityLevels;

				return true;
			}
		}
	}

	return false;
}

void CGraphic_Device::ChooseBestMSAAMode(D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt, BOOL windowed, D3DMULTISAMPLE_TYPE& type, DWORD& qualityLevels, DWORD& samplesPerPixel)
{
	bool supported = false;

	struct MSAAMode
	{
		D3DMULTISAMPLE_TYPE type;
		DWORD samples;
	}
	multsamplingTypes[15] =
	{
		{ D3DMULTISAMPLE_16_SAMPLES,  16 },
		{ D3DMULTISAMPLE_15_SAMPLES,  15 },
		{ D3DMULTISAMPLE_14_SAMPLES,  14 },
		{ D3DMULTISAMPLE_13_SAMPLES,  13 },
		{ D3DMULTISAMPLE_12_SAMPLES,  12 },
		{ D3DMULTISAMPLE_11_SAMPLES,  11 },
		{ D3DMULTISAMPLE_10_SAMPLES,  10 },
		{ D3DMULTISAMPLE_9_SAMPLES,   9 },
		{ D3DMULTISAMPLE_8_SAMPLES,   8 },
		{ D3DMULTISAMPLE_7_SAMPLES,   7 },
		{ D3DMULTISAMPLE_6_SAMPLES,   6 },
		{ D3DMULTISAMPLE_5_SAMPLES,   5 },
		{ D3DMULTISAMPLE_4_SAMPLES,   4 },
		{ D3DMULTISAMPLE_3_SAMPLES,   3 },
		{ D3DMULTISAMPLE_2_SAMPLES,   2 }
	};

	for (int i = 0; i < 15; ++i)
	{
		type = multsamplingTypes[i].type;

		supported = MSAAModeSupported(type, backBufferFmt, depthStencilFmt,
			windowed, qualityLevels);

		if (supported)
		{
			samplesPerPixel = multsamplingTypes[i].samples;
			return;
		}
	}

	type = D3DMULTISAMPLE_NONE;
	qualityLevels = 0;
	samplesPerPixel = 1;
}
