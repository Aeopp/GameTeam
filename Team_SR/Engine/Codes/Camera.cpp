#include "..\Headers\Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice) {}

const CAMERA_DESC & CCamera::GetCameraDesc() const
{
	return m_CameraDesc;
}

HRESULT CCamera::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::ReadyGameObject(void* pArg/* = nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CCamera::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CCamera::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	/* 뷰행렬 만드는 함수 */
	D3DXMatrixLookAtLH(
		&m_CameraDesc.matView, /* 반환 값 */
		&m_CameraDesc.vEye, /* 월드상 카메라 위치 Eye */
		&m_CameraDesc.vAt, /* 카메라가 바라보는 지점 At */
		&m_CameraDesc.vUp /* Up벡터 */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView)))
		return E_FAIL;

	/* 원근 투영 행렬을 만드는 함수 */
	D3DXMatrixPerspectiveFovLH(
		&m_CameraDesc.matProj, /* 반환 값 */
		D3DXToRadian(m_CameraDesc.fFovY), /* 시야각 FovY */
		m_CameraDesc.fAspect, /* 종횡비 Aspect */
		m_CameraDesc.fNear, /* 카메라와 근편명과의 Z거리 Near */
		m_CameraDesc.fFar /* 카메라와 원평면과의 Z거리 Far */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj)))
		return E_FAIL;

	return _uint();
}

HRESULT CCamera::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CCamera::Free()
{
	CGameObject::Free();
}
