#include "stdafx.h"
#include "..\Headers\Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr)
{
}

HRESULT CBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		// 2020.12.16 17:35 KMJ
		// 아규먼트 메모리 해제
		// 구조체 크기 검사
		if (sizeof(BulletBasicArgument) == *static_cast<_uint*>(pArg)) {
			BulletBasicArgument* pArgument = static_cast<BulletBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_vLook = pArgument->vDir;
			delete pArg;
		}
	}

	return S_OK;
}

_uint CBullet::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CBullet::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	return _uint();
}

HRESULT CBullet::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::AddComponents()
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

// 텍스처 프레임 이동
void CBullet::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

void CBullet::Free()
{
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}

