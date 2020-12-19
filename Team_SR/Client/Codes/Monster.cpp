#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Camera.h"
#include "NormalUVVertexBuffer.h"

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

	const auto& _TransformDesc =m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);

	/*_matrix matBillboardY, matView;
	D3DXMatrixIdentity(&matBillboardY);
	matView = pCamera->GetCameraDesc().matView;

	matBillboardY._11 = matView._11;
	matBillboardY._13 = matView._13;
	matBillboardY._31 = matView._31;
	matBillboardY._33 = matView._33;

	D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

	m_pTransformCom->m_TransformDesc.matWorld *= matBillboardY;*/



	return S_OK;
}


HRESULT CMonster::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			auto iter_find = m_mapTexture.find(m_wstrTextureKey);
			if (m_mapTexture.end() == iter_find)
				return E_FAIL;
			CTexture* pTexture = (CTexture*)iter_find->second;
			IDirect3DBaseTexture9* const  DiffuseTexture = pTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", false);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", false);
		}
		// 2. 세팅을 하고 난 이후의                                   ↑↑↑↑↑↑↑↑↑↑     TRUE 로 바꾸어주기.
		{
			// m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),SpecularTexture);
			// m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),NormalTexture);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();

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

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
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
	m_stStatus.fHP -= 1.f;
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
	/// <summary> 2020 12 20 이호준
	 SafeRelease(_VertexBuffer);
	/// </summary>
	
	SafeRelease(m_pVIBufferCom);		// 버텍스 버퍼
	for (auto& rPair : m_mapTexture)	// map 텍스처 릴리즈
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	CGameObject::Free();
}


