#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Package.h"

USING(Engine)
class ImGuiHelper
{
public:
	static void Init(HWND _Hwnd , IDirect3DDevice9* _Device)noexcept;
	static void UpdateStart();
	static void Update();
	static void UpdateEnd();
	static void CheckBoxCall();
	/*Render 함수는 반드시 백버퍼,깊이,스텐실버퍼 Clear 이후에
	 Device::BeginScene 와 Device::EndScene 사이에 호출 해야함 !!*/
	static void Render(IDirect3DDevice9* _Device);
	static void ShutDown()noexcept;
    static void ResetDevice
		(IDirect3DDevice9* Device, 
        D3DPRESENT_PARAMETERS& D3Dpp, 
        WPARAM wParam, 
        LPARAM lParam)noexcept;
	
	static bool bInitialize;
	static bool bEditOn;
	static bool bDemo;
	static bool bPackageEdit;
	static bool bMapPackageEdit;
public:
	static void DebugInfo(HWND _Hwnd);
	static void Picking(IDirect3DDevice9* const _Device ,const std::vector<PlaneInfo>& _PlaneInfo);
	static void Save();
private:
	static ID3DXMesh* _SphereMesh;
	static PackageContainer _PackageContainer;
	static vec4 PackageDiffuseColor;
	static float Radius;
};

