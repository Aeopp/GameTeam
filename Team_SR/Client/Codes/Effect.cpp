#include "stdafx.h"
#include "..\Headers\Effect.h"
#include "Camera.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_fFrameSpeed(0.f), m_pTexture(nullptr)
{
}

HRESULT CEffect::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::ReadyGameObject(void * pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(CEffect::AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		// 구조체 크기 검사
	}

	return S_OK;
}

_uint CEffect::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CEffect::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);



	return _uint();
}

HRESULT CEffect::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

bool CEffect::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fEndFrame;
		return true;
	}
	return false;
}

HRESULT CEffect::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);



	return S_OK;
}

void CEffect::Free()
{
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}
