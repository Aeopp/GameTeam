#include "stdafx.h"
#include "..\Headers\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	m_vecDir = _vector(0.f, -1.f, 1.f);
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_fDistance = 1;

	return S_OK;
}

HRESULT CCamera::ReadyGameObject(void * pArg/* = nullptr*/)
{
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
	return _uint();
}

HRESULT CCamera::RenderGameObject()
{
	return S_OK;
}

CCamera * CCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;
	CCamera* pInstance = new CCamera(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CCamera");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera::Clone(void * pArg /*= nullptr*/)
{
	return nullptr;
}

void CCamera::Free()
{
	CGameObject::Free();
}

HRESULT CCamera::AddComponent()
{
	return S_OK;
}

HRESULT CCamera::SetViewMatrix(TRANSFORM_DESC tTrans)
{
	D3DXMATRIX matView,matRotY;

	m_vecDir = _vector(0.f, -1.f, 1.f);
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(tTrans.vRotation.y));
	D3DXVec3TransformNormal(&m_vecDir, &m_vecDir, &matRotY);
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);

	D3DXMatrixLookAtLH(
		&matView, /* 반환 값 */
		&D3DXVECTOR3(tTrans.vPosition - m_vecDir * m_fDistance), /* 월드상 카메라 위치 Eye */
		&tTrans.vPosition, /* 카메라가 바라보는 지점 At */
		&D3DXVECTOR3(0.f, 1.f, 0.f) /* Up벡터 */);

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matView)))
		return E_FAIL;
}
