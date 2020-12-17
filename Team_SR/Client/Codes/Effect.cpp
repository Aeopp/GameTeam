#include "stdafx.h"
#include "..\Headers\Effect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr)
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

void CEffect::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

void CEffect::Free()
{
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}
