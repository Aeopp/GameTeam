#include "stdafx.h"
#include "ImGuiHelper.h"

bool ImGuiHelper::bInitialize = false;
bool ImGuiHelper::bEditOn = true;
bool ImGuiHelper::bDemo = false;

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
	if (ImGuiHelper::bEditOn)
	{
		ImGui::ShowDemoWindow(&bEditOn);
	}
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
};


void ImGuiHelper::DebugInfo(HWND _Hwnd)
{
	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("Debug Info", &ImGuiHelper::bEditOn);
		{
			POINT _MousePt;
			GetCursorPos(&_MousePt);
			ScreenToClient(_Hwnd, &_MousePt);
			ImGui::Text("Mouse Pos");
			ImGui::Separator();
			ImGui::Text("X : %d Y : %d", _MousePt.x, _MousePt.y);
		}
		ImGui::End();
	}
}

void ImGuiHelper::Text()
{
	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("How To Use ImGui", &ImGuiHelper::bEditOn);
		{
			ImGui::CollapsingHeader("How To Use ImGui");
			// ImGui의 텍스트 함수들은 C의 Printf처럼 가변인자 함수여서 printf 과 똑같은 포맷팅으로 텍스트 출력 가능
			ImGui::Text("Example Code => %s ", "ImGuiHelper.h");
			ImGui::Spacing();
			ImGui::BulletText("BulletText%s", "TextTextTextText");
			// 구분자이후에 한줄 건너뜀
			ImGui::Separator();
			ImGui::Text("TextTextText");
			// 텍스트를 또 입력하더라도 같은라인에 입력
			ImGui::SameLine();
			ImGui::Text("TextTextText2222222");
		}
		ImGui::End();
	}
}

void ImGuiHelper::Slider()
{
	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("Slider", &ImGuiHelper::bEditOn);
		{
			static float FloatValue = 0;
			static constexpr float FloatMin = -100.f;
			static constexpr float FloatMax = +100.f;
			// 변수 포인터와 Min Max 입력
			ImGui::SliderFloat("FloatEdit", &FloatValue, FloatMin, FloatMax);

			static int IntValue = 0;
			static constexpr int IntMin = -100;
			static constexpr int IntMax = +100;

			ImGui::SliderInt("IntEdit", &IntValue, IntMin, IntMax);
		}
		ImGui::End();
	}
}

void ImGuiHelper::CheckBox()
{
}

void ImGuiHelper::Button()
{
	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("Button", &ImGuiHelper::bEditOn);
		{
			static int ClickCount = 0;
			// 버튼 체크는 반환값으로 체크하며 true 일때 실행될 함수를 정해주면 됨.
			if (ImGui::Button("Click Here"))
			{
				// Function Call
				++ClickCount;
			}
			ImGui::Text("Click Count : %d", ClickCount);
		}
		ImGui::End();
	}
}


