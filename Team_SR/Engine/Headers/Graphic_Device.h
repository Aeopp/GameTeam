#pragma once
#ifndef __GRAPHIC_DEVICE_H__

#include "Base.h"

BEGIN(Engine)
class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() =default;
public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPDIRECT3D9 Get_SDK() { return m_pSDK; };
	D3DPRESENT_PARAMETERS& GetD3Dpp() { return D3Dpp; };
	FORCEINLINE ID3DXLine& GetLine() { return *_Line; };
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, EDisplayMode eDisplayMode);
public:
	virtual void Free() override;
private:
	ID3DXLine* _Line;
	D3DPRESENT_PARAMETERS D3Dpp;
	LPDIRECT3D9 m_pSDK; // 그래픽카드의 수준을 조사하고 생성하는 객체. 
	LPDIRECT3DDEVICE9 m_pDevice; // 애가 실질적으로 그래픽 장치를 제어하는 객체. 

    bool MSAAModeSupported(D3DMULTISAMPLE_TYPE type, D3DFORMAT backBufferFmt,
        D3DFORMAT depthStencilFmt, BOOL windowed,
		DWORD& qualityLevels);

    void ChooseBestMSAAMode(D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt,
        BOOL windowed, D3DMULTISAMPLE_TYPE& type,
		DWORD& qualityLevels, DWORD& samplesPerPixel);


	// 장치를 생성하는 과정. 

	//1. 장치를 제어하기 위한 객체를 생성하는 객체(m_pSdk)를 생성. 
	//2. 장치의 수준을 조사해야한다. 
	//3. 수준에 맞는 장치를 제어하기 위한 객체 생성. 

	/*
	com - component Object Model의 약자. 
	우리가 배우고 있는 다이렉트에서는 장치 혹은 어떠한 데이터를 다루는 기능들을 하나의 부품처럼 제공해주고 있다. 
	그리고 사용자는 이 부품들을 이용하여 마치 레고를 조립하듯 프로그램을 구성해 나가야 한다. 
	*/
};
END

#define __GRAPHIC_DEVICE_H__
#endif