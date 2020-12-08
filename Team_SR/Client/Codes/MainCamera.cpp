#include "stdafx.h"
#include "MainCamera.h"
#include "MyMath.h"

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

	return _uint();
}

_uint CMainCamera::LateUpdateGameObject(float fDeltaTime)
{
	static const _vector WorldUp = { 0,1,0 };
	const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;

	m_CameraDesc.vEye = _TransformDesc.vPosition;
	_vector Look{ 0,0,1 };

	Look = MATH::RotationVec(Look, MATH::AxisX, _TransformDesc.vRotation.x);
	Look = MATH::RotationVec(Look, MATH::AxisY, _TransformDesc.vRotation.y);

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
}
