#include "stdafx.h"
#include "..\Headers\HellBossRingBullet.h"


CHellBossRingBullet::CHellBossRingBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CHellBossRingBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossRingBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 1.f,1.f,1.f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 100.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;
	m_fFrameSpeed = 30.f;

	return S_OK;
}

_uint CHellBossRingBullet::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	// 스케일
	m_pTransformCom->m_TransformDesc.vScale.x += 1.f * fDeltaTime;
	m_pTransformCom->m_TransformDesc.vScale.y += 1.f * fDeltaTime;

	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);
	Bullet_Attack(m_stStatus.fATK);	// 플레이어 충돌 처리

	return _uint();
}

_uint CHellBossRingBullet::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossRingBullet::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CHellBossRingBullet::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

HRESULT CHellBossRingBullet::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 리틀 데몬 원형 총알 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_RingBullet",
		L"Com_Texture_LittleDemon_RingBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

CHellBossRingBullet* CHellBossRingBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossRingBullet* pInstance = new CHellBossRingBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossRingBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossRingBullet::Clone(void* pArg/* = nullptr*/)
{
	CHellBossRingBullet* pClone = new CHellBossRingBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossRingBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossRingBullet::Free()
{
	CBullet::Free();
}
