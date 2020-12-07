#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


USING(Engine)
class ImGuiHelper
{
public:
	static void Init(HWND _Hwnd , IDirect3DDevice9* _Device)noexcept;
	static void UpdateStart();
	static void UpdateEnd();
	/*Render 함수는 반드시 백버퍼,깊이,스텐실버퍼 Clear 이후에
	 Device::BeginScene 와 Device::EndScene 사이에 호출 해야함 !!*/
	static void Render();
	static void ShutDown()noexcept;
    static void ResetDevice
		(IDirect3DDevice9* Device, 
        D3DPRESENT_PARAMETERS& D3Dpp, 
        WPARAM wParam, 
        LPARAM lParam)noexcept;
	
	static bool bInitialize;
	static bool bEditOn;
	static bool bDemo;
public:
	/*Example Code Here*/
	static void Text();
	static void Slider();
	static void CheckBox();
	static void Button();
};

