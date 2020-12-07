#include "stdafx.h"
#include "ImGuiHelper.h"

bool ImGuiHelper::bInitialize = false;
bool ImGuiHelper::bEditOn = false;

void ImGuiHelper::Init(HWND _Hwnd,
	IDirect3DDevice9* _Device)noexcept 
{
#pragma region IMGUI_INIT
	{
		assert(!bInitialize);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& ImGui_IO = ImGui::GetIO();
		ImGui_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsClassic();
		ImGui_ImplWin32_Init(g_hWnd);
		ImGui_ImplDX9_Init(_Device);
		bInitialize = true;
	}
#pragma endregion
}

void ImGuiHelper::UpdateStart()
{
#pragma region ImGui_FrameStart
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
#pragma endregion
	ImGui::ShowDemoWindow(&bEditOn);
}

void ImGuiHelper::UpdateEnd()
{
	ImGui::EndFrame();
}

void ImGuiHelper::Render()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::ShutDown()noexcept 
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiHelper::ResetDevice(IDirect3DDevice9* Device, D3DPRESENT_PARAMETERS& D3Dpp, WPARAM wParam,
	LPARAM lParam) noexcept
{
	if (Device != NULL && wParam != SIZE_MINIMIZED)
	{
		D3Dpp.BackBufferWidth = LOWORD(lParam);
		D3Dpp.BackBufferHeight = HIWORD(lParam);
		ImGui_ImplDX9_InvalidateDeviceObjects();
		HRESULT hr = Device->Reset(&D3Dpp);
		if (hr == D3DERR_INVALIDCALL)
			IM_ASSERT(0);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void Example::Text()
{
	ImGui::Begin(__FUNCTION__, &ImGuiHelper::bEditOn);
	{
		// printf 처럼 가변인자 함수여서 printf 과 똑같은 포맷팅으로 텍스트 출력 가능
		
		ImGui::CollapsingHeader("ImGui::CollapsingHeader");
		ImGui::Text("BlaBlaBlaBla%d", 7777);
		ImGui::Spacing();
		ImGui::BulletText("BulletText%s", "TextTextTextText");
		// 구분자이후에 한줄 건너뜀
		ImGui::Separator();
	}
	ImGui::End();
}

void Example::Slider()
{
}


