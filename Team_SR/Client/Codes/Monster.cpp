#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Camera.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_fFrameSpeed(10.f)
	, m_fCrossValue(0.f), m_vCollisionDir{0.f, 0.f, 0.f}, m_vAim {0.f, 0.f, 0.f}
	, m_pPlayer(nullptr), m_stOriginStatus{}, m_stStatus{}
	, m_bFrameLoopCheck(false), m_byMonsterFlag(0)
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

	if (FAILED(CMonster::AddComponents()))
		return E_FAIL;
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

	return S_OK;
}

_uint CMonster::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	// 2020.12.17 11:08 KMJ
	// 충돌 이동
	CollisionMovement(fDeltaTime);

	return _uint();
}

_uint CMonster::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	IsBillboarding();

	return _uint();
}

HRESULT CMonster::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	_matrix matBillboardY, matView;
	D3DXMatrixIdentity(&matBillboardY);
	matView = pCamera->GetCameraDesc().matView;

	matBillboardY._11 = matView._11;
	matBillboardY._13 = matView._13;
	matBillboardY._31 = matView._31;
	matBillboardY._33 = matView._33;

	D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

	m_pTransformCom->m_TransformDesc.matWorld *= matBillboardY;



	return S_OK;
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

// 몬스터가 피해를 받음
void CMonster::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	CGameObject::Hit(_Target, _CollisionInfo);

	// 2020.12.17 10:44 KMJ
	// 공평회에서는 일단 고정임
	//m_stStatus.fHP -= fDemage;
	m_stStatus.fHP -= 5.f;
}

// 텍스처 프레임 이동 - 프레임 카운트가 End에 도달하면 true, 아니면 false
bool CMonster::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
		return true;
	}
	return false;
}

// 플레이어 인식 - 인식하면 true, 인식하지 못하면 false
bool CMonster::PlayerAwareness()
{
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	float fDis = D3DXVec3Length(&vDir);
	// 플레이어가 범위 안에 있으면
	if (fDis <= m_stStatus.fDetectionRange) {
		return true;
	}
	return false;
}

// 플레이어가 가까이 근접해있는가 - 가까우면 true, 아니면 false
bool CMonster::PlayerBeNear()
{
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	float fDis = D3DXVec3Length(&vDir);
	// 플레이어가 범위 안에 있으면
	if (fDis <= m_stStatus.fMeleeRange) {
		return true;
	}
	return false;
}

// 2020.12.17 11:08 KMJ
// 충돌 이동
void CMonster::CollisionMovement(float fDeltaTime)
{
	// 개쌉 야매
	m_fCrossValue -= 10.f * fDeltaTime;
	if (m_fCrossValue < 0.f)
		m_fCrossValue = 0.f;
	m_pTransformCom->m_TransformDesc.vPosition += m_vCollisionDir * m_fCrossValue * fDeltaTime;
}

void CMonster::Free()
{
	SafeRelease(m_pVIBufferCom);		// 버텍스 버퍼
	for (auto& rPair : m_mapTexture)	// map 텍스처 릴리즈
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	CGameObject::Free();
}


