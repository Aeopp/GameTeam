#include "stdafx.h"
#include "ImGuiHelper.h"
#include "DXWrapper.h"
#include "CollisionComponent.h"

bool ImGuiHelper::bInitialize = false;
bool ImGuiHelper::bEditOn = true;
bool ImGuiHelper::bDemo = false;
bool ImGuiHelper::bPackageEdit = false;

ID3DXMesh *  ImGuiHelper::_SphereMesh{ nullptr };
PackageContainer ImGuiHelper::_PackageContainer{};

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
	D3DXCreateSphere(_Device, 1.f, 25, 25, &_SphereMesh, 0);
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
};
void ImGuiHelper::Render(IDirect3DDevice9* _Device)
{
	if (!CManagement::Get_Instance()->bDebug)return;

	DWORD _AlphaValue;
	_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &_AlphaValue);
	if (_AlphaValue == TRUE)
		_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	bool bOpen = (ImGuiHelper::bEditOn && ImGuiHelper::bPackageEdit);
	if (bOpen)
	{
		for (const auto& _CurPackege : _PackageContainer._PackageVec)
		{
			_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			_Device->SetVertexShader(nullptr);
			_Device->SetPixelShader(nullptr);
			mat DebugSphereWorld = MATH::WorldMatrix(
				_CurPackege.Scale,
				_CurPackege.Rotation,
				_CurPackege.Location);
			_Device->SetTransform(D3DTS_WORLD, &DebugSphereWorld);
			_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

			_SphereMesh->DrawSubset(0);
			_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		};
	}
	

	if (_AlphaValue == TRUE)
		_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::ShutDown()noexcept 
{
	SafeRelease(_SphereMesh);

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

		bool bOpen = (ImGuiHelper::bEditOn  &&ImGuiHelper::bPackageEdit);
		ImGui::Begin("PackageEdit", &bOpen);
		ImGui::Separator();
		ImGui::SliderFloat3("Scale",(float*)&_PackageContainer._CurEditPackage.Scale,1.f, 100.f);
		ImGui::SliderFloat3("Rotation",(float*)&_PackageContainer._CurEditPackage.Rotation,-360.f, +360.f);
		ImGui::SliderFloat3("Location",(float*)&_PackageContainer._CurEditPackage.Location,-1000.f, +1000.f);
		static char TextBuf[MAX_PATH];
		ImGui::InputText("Name Here ", TextBuf, MAX_PATH);_PackageContainer._CurEditPackage.Name = TextBuf;
		ImGui::Separator();
		static char FileName[MAX_PATH];
		ImGui::InputText("File Name ", FileName, MAX_PATH);
		if (ImGui::Button("Save"))
		{
			_PackageContainer.CurInfoFileRecord(FileName);
		}
		ImGui::End();
	}
};

void ImGuiHelper::Picking(IDirect3DDevice9* const _Device,
	const std::vector<PlaneInfo>& _PlaneInfo)
{
	bool bOpen = (ImGuiHelper::bEditOn && ImGuiHelper::bPackageEdit);

	if (bOpen)
	{
		POINT MousePt;
		GetCursorPos(&MousePt);
		ScreenToClient(g_hWnd, &MousePt);
		const vec3 MouseVec = { (float)MousePt.x,(float)MousePt.y ,0.f };
		Ray _Ray = MATH::GetRayScreenProjection(MouseVec, _Device, (float)WINCX, (float)WINCY);

		bool Intersect = false;

		float TFinal = FLT_MAX;
		vec3 IntersectPointFinal;

		for (const auto& _CurTriangle : _PlaneInfo)
		{
			float t = 0;
			vec3 IntersectPoint;
			if (Collision::IsTriangleToRay(_CurTriangle, _Ray, t, IntersectPoint))
			{
				Intersect = true;
				if (t <= TFinal)
				{
					IntersectPointFinal = IntersectPoint;
					TFinal = t;
				}
			}
		}

		if (Intersect)
		{
			_PackageContainer._CurEditPackage.Location = IntersectPointFinal;
			_PackageContainer.CurInfoPush();

			{
				MyLight _Light;

				_Light.Diffuse = { 1,1,1 };
				_Light.Location = IntersectPointFinal;
				_Light.Radius = 10;

				Effect::RegistLight(_Light);
			}

		}
		}
	
}

void ImGuiHelper::Save()
{
	_PackageContainer.CurInfoPush();
}

