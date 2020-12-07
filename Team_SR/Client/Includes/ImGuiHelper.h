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
	/*Render �Լ��� �ݵ�� �����,����,���ٽǹ��� Clear ���Ŀ�
	 Device::BeginScene �� Device::EndScene ���̿� ȣ�� �ؾ��� !!*/
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

