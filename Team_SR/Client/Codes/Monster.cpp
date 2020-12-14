#include "stdafx.h"
#include "..\Headers\Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f)
	, m_pPlayer(nullptr), m_stOriginStatus{}, m_stStatus{}
	, m_bFrameLoopCheck(false)
{
}

HRESULT CMonster::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

<<<<<<< HEAD
	if (FAILED(CMonster::AddComponents()))
		return E_FAIL;
=======
	if (nullptr != pArg)
	{
		// 구조체 크기 검사
		if (sizeof(MonsterBasicArgument) == *static_cast<_uint*>(pArg))
		{
			MonsterBasicArgument* pArgument = static_cast<MonsterBasicArgument*>(pArg);
			m_pPlayer = reinterpret_cast<CGameObject*>(pArgument->pPlayer);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
		}
	}
>>>>>>> MyeongJun

	return S_OK;
}

_uint CMonster::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CMonster::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	return _uint();
}

HRESULT CMonster::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::AddComponents()
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

// 텍스처 프레임 이동 - 프레임 카운트가 End에 도달하면 true, 아니면 false
bool CMonster::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
		return true;
	}
	return false;
}

// 플레이어 인식 - 인식하면 TRUE, 인식하지 못하면 FALSE
bool CMonster::PlayerAwareness()
{
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	float fDis = D3DXVec3Length(&vDir);
	// 플레이어가 범위 안에 있으면
	if (fDis <= m_stStatus.fDetectionDistance) {
		return true;
	}
	return false;
}

void CMonster::Free()
{
	SafeRelease(m_pVIBufferCom);
	// TODO :: 텍스쳐 컴포넌트 추가 이후 주석 풀기
	//SafeRelease(m_pTextureCom);

	CGameObject::Free();
}


