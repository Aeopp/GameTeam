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
	m_CameraDesc.fFar= 1000.f;
	m_CameraDesc.fNear=1.f;
	m_CameraDesc.fAspect= static_cast<float>(WINCX) / WINCY;
	m_CameraDesc.vUp = { 0,1,0 };
}

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

	ImGui::Begin("CameraEdit",&ImGuiHelper::bEditOn);
	ImGui::Separator();
	ImGui::SliderFloat("FOV", &m_CameraDesc.fFovY, 0.f, 180.f,"Deg : %f");
	ImGui::Separator();
	ImGui::Text("ShakeEdit");
	static float Duration = 1.f;
	static float Force = 1.f;
	static float Vibration = 1.f;
	static vec3 AxisScale{ 1,1,1 };
	if (ImGui::Button("Shake!"))
	{
		Shake(Duration, Force, AxisScale,Vibration );
	}
	ImGui::SliderFloat("Duration", &Duration, 0.1f, 10.f);
	ImGui::SliderFloat("Force", &Force, 1.f, 10000.f);
	ImGui::SliderFloat("Vibration", &Vibration, 1.f, 10000.f);

	ImGui::SliderFloat3("AxisScale", reinterpret_cast<float*>(&AxisScale), 0.f, 1,
		"%f", 1.f);
	ImGui::End();
	
	return _uint();
}

_uint CMainCamera::LateUpdateGameObject(float fDeltaTime)
{
	static const _vector WorldUp = { 0,1,0 };

	auto* _Transform =
		dynamic_cast<CTransform*>(m_pManagement->GetComponent(-1, 
			CLayer::Tag + TYPE_NAME<CPlayer>(),
		CComponent::Tag + TYPE_NAME<CTransform>(), 0));

	//m_pManagement->GetCurrentScene();
	m_pTransformCom->m_TransformDesc= _Transform->m_TransformDesc;
	Shaking(fDeltaTime);
	m_CameraDesc.vEye = m_pTransformCom->m_TransformDesc.vPosition;
	_vector Look{ 0,0,1 };

	Look = MATH::RotationVec(Look, MATH::AxisX, m_pTransformCom->m_TransformDesc.vRotation.x);
	Look = MATH::RotationVec(Look, MATH::AxisY, m_pTransformCom->m_TransformDesc.vRotation.y);
	Look = MATH::RotationVec(Look, MATH::AxisZ, m_pTransformCom->m_TransformDesc.vRotation.z);

	// 올바른 외적을 위해 Up과 Look 가 같을 경우 살짝 보정
	if (Look == WorldUp) 
	{
		Look.y -= (std::numeric_limits<float>::min)();
	}

	_vector Right{};
	D3DXVec3Cross(&Right, &WorldUp, &Look);
	D3DXVec3Cross(&m_CameraDesc.vUp, &Look, &Right);

	m_CameraDesc.vAt = m_CameraDesc.vEye + (Look * 100);

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

