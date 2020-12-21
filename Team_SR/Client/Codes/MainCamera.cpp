#include "stdafx.h"
#include "MainCamera.h"
#include "MyMath.h"
#include "ImGuiHelper.h"
#include "Layer.h"
#include "Player.h"

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
{
	m_CameraDesc.fFovY = 45.f;
	m_CameraDesc.fFar = 1000.f;
	m_CameraDesc.fNear = 1.f;
	m_CameraDesc.fAspect = static_cast<float>(WINCX) / WINCY;
	m_CameraDesc.vUp = { 0,1,0 };
};

HRESULT CMainCamera::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainCamera::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;
	m_pTransformCom->m_TransformDesc.vPosition = { 0,0,-10 };

	return S_OK;
}

_uint CMainCamera::UpdateGameObject(float fDeltaTime)
{
	m_pTransformCom->UpdateTransform();

	CameraEditProcess();

	if (!bThirdPerson) 
	{ 
	
	}
	
	return _uint();
}

_uint CMainCamera::LateUpdateGameObject(float fDeltaTime)
{
	static const _vector WorldUp = { 0,1,0 };

	auto* _PlayerTransform =
		dynamic_cast<CTransform*>(m_pManagement->GetComponent(-1, 
			CLayer::Tag + TYPE_NAME<CPlayer>(),
		CComponent::Tag + TYPE_NAME<CTransform>(), 0));

	m_pTransformCom->m_TransformDesc = _PlayerTransform->m_TransformDesc;

	if (!bThirdPerson)
	{
		Shaking(fDeltaTime);

		ShowCursor(true);

		POINT _MousePt;
		GetCursorPos(&_MousePt);
		ScreenToClient(g_hWnd, &_MousePt);

		const vec3 ScreenCenter = { static_cast<float> (WINCX) / 2.f,static_cast<float>(WINCY) / 2.f,0.f };
		const vec3 CurrentMousePosition = { static_cast<float>(_MousePt.x), static_cast<float>(_MousePt.y) ,0.f };
		const vec3 MousePositionDelta = CurrentMousePosition - ScreenCenter;

		auto& TransformDesc = m_pTransformCom->m_TransformDesc;
		TransformDesc.vRotation.y += MousePositionDelta.x *fDeltaTime *FirstPersonRotationSpeed;
		TransformDesc.vRotation.x += MousePositionDelta.y * fDeltaTime * FirstPersonRotationSpeed;

		POINT ScreenCenterPT = { static_cast<LONG>(ScreenCenter.x) , static_cast<LONG>(ScreenCenter.y) };
		ClientToScreen(g_hWnd, &ScreenCenterPT);
		SetCursorPos(ScreenCenterPT.x, ScreenCenterPT.y);

		
		_vector Look{ 0,0,1 };

		Look = MATH::RotationVec(Look, MATH::AxisX, m_pTransformCom->m_TransformDesc.vRotation.x);
		Look = MATH::RotationVec(Look, MATH::AxisY, m_pTransformCom->m_TransformDesc.vRotation.y);
		Look = MATH::RotationVec(Look, MATH::AxisZ, m_pTransformCom->m_TransformDesc.vRotation.z);
		
		if (Look == WorldUp)
		{
			Look.y -= (std::numeric_limits<float>::min)();
		}

		_vector Right{};
		D3DXVec3Cross(&Right, &WorldUp, &Look);

		m_CameraDesc.vUp = MATH::Cross(Look, Right);
		m_CameraDesc.vEye = m_pTransformCom->m_TransformDesc.vPosition;
		m_CameraDesc.vAt = m_CameraDesc.vEye + (Look * 100.f);

		_PlayerTransform->m_TransformDesc.vRotation = TransformDesc.vRotation;
	}
	else if (bThirdPerson)
	{
		ShowCursor(true);

		Shaking(fDeltaTime);

		vec3 Location = Offset;
		m_CameraDesc.vAt = _PlayerTransform->m_TransformDesc.vPosition;
		Location = MATH::RotationVec(Location, { 1,0,0 }, OffsetRotation.x);
		Location =  MATH::RotationVec(Location, { 0,1,0 }, OffsetRotation.y);
		Location = MATH::RotationVec(Location, { 0,0,1 }, OffsetRotation.z);
		
		m_CameraDesc.vEye = Location + m_CameraDesc.vAt;
		vec3 Look = m_CameraDesc.vAt - m_CameraDesc.vEye;
		if (Look == WorldUp)
			Look.y -= 0.0001f;

		Look=MATH::Normalize(Look);
		vec3 Right = MATH::Cross(WorldUp,Look);
		m_CameraDesc.vUp = MATH::Cross(Look, Right);
	}

	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CMainCamera::RenderGameObject()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	return S_OK;
}



CMainCamera* CMainCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMainCamera* pInstance = new CMainCamera(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CTerrain");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMainCamera::Clone(void * pArg)
{
	CMainCamera* pClone = new CMainCamera(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMainCamera::Free()
{
	Super::Free();
};

void CMainCamera::Shake(const float Duration, const float Force, vec3 AxisScale,
	const float Vibration)
{
	ShakeInfo _Info;
	_Info.Duration = Duration;
	_Info.Force = Force;
	_Info.AxisScale = AxisScale;
	_Info.Vibration = Vibration;

	float lhs = _CurrentShake.Duration * _CurrentShake.Force *  _CurrentShake.Vibration *
					_CurrentShake.AxisScale.x * _CurrentShake.AxisScale.y * 
					_CurrentShake.AxisScale.z;

	float rhs = _Info.Duration * _Info.Force*       _Info.Vibration*_Info.AxisScale.x 
					*_Info.AxisScale.y*_Info.AxisScale.z;

	if (rhs > lhs)
	{
		_CurrentShake = std::move(_Info);
		_CurrentShake.Goal = m_pTransformCom->m_TransformDesc.vPosition;
	}
}

void CMainCamera::CameraEditProcess()&
{
	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("CameraEdit", &ImGuiHelper::bEditOn);
		ImGui::Checkbox("3rdPerson", &bThirdPerson);
		ImGui::Separator();
		ImGui::SliderFloat("FOV", &m_CameraDesc.fFovY, 0.f, 180.f, "Deg : %f");
		ImGui::Separator();
		ImGui::Text("ShakeEdit");
		static float Duration = 1.f;
		static float Force = 1.f;
		static float Vibration = 1.f;
		static vec3 AxisScale{ 1,1,1 };

		if (ImGui::Button("Shake!"))
		{
			Shake(Duration, Force, AxisScale, Vibration);
		}

		ImGui::SliderFloat("Duration", &Duration, 0.1f, 10.f);
		ImGui::SliderFloat("Force", &Force, 1.f, 10000.f);
		ImGui::SliderFloat("Vibration", &Vibration, 1.f, 10000.f);
		ImGui::SliderFloat3("AxisScale", reinterpret_cast<float*>(&AxisScale), 0.f, 1, "%f", 1.f);

		if (bThirdPerson)
		{
			ImGui::Text("Offset Only 3Person");
			ImGui::Separator();
			ImGui::Text("Location");
			ImGui::Separator();
			ImGui::SliderFloat("X", &Offset.x, -50, +50);
			ImGui::SliderFloat("Y", &Offset.y, -50, +50);
			ImGui::SliderFloat("Z", &Offset.z, -50, +50);

			ImGui::Text("Rotation");
			ImGui::Separator();
			ImGui::SliderFloat("RX", &OffsetRotation.x, -360, +360, "Deg :%f");
			ImGui::SliderFloat("RY", &OffsetRotation.y, -360, +360, "Deg :%f");
			ImGui::SliderFloat("RZ", &OffsetRotation.z, -360, +360, "Deg :%f");
		}
		else
		{
			const auto& TransformDesc = m_pTransformCom->m_TransformDesc;

			ImGui::Text("Rotation Only 1Person");
			ImGui::Separator();
			ImGui::Text("Location");
			ImGui::Separator();
			ImGui::Text("X : %f ", TransformDesc.vPosition.x);
			ImGui::Text("Y : %f ", TransformDesc.vPosition.y);
			ImGui::Text("Z : %f ", TransformDesc.vPosition.z);

			ImGui::Text("Rotation");
			ImGui::Separator();
			ImGui::Text("X : %f ", TransformDesc.vRotation.x);
			ImGui::Text("Y : %f ", TransformDesc.vRotation.y);
			ImGui::Text("Z : %f ", TransformDesc.vRotation.z);
		}


		ImGui::End();
	}

}

void CMainCamera::Shaking(const float DeltaTime)
{
	_CurrentShake.Duration -= DeltaTime;

	if (_CurrentShake.Duration < 0.f)
	{
		return;
	}

	 vec3& Location = m_pTransformCom->m_TransformDesc.vPosition;
	const float Distance = MATH::Length(Location - _CurrentShake.Goal);

	if (Distance < 1.f)
	{
		vec3 Vibration = MATH::RandVec() * _CurrentShake.Vibration;

		Vibration.x *= _CurrentShake.AxisScale.x;
		Vibration.y *= _CurrentShake.AxisScale.y;
		Vibration.z *= _CurrentShake.AxisScale.z;

		_CurrentShake.Goal = Vibration + Location;
	}
	else
	{
		vec3 Dir = MATH::Normalize(_CurrentShake.Goal - Location);
		Location += Dir * DeltaTime * _CurrentShake.Force;
	}
}

