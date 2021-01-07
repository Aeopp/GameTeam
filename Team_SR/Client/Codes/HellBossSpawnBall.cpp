#include "stdafx.h"
#include "..\Headers\HellBossSpawnBall.h"
#include "Player.h"


CHellBossSpawnBall::CHellBossSpawnBall(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CHellBossSpawnBall::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossSpawnBall::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 15.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 50.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;
	m_fFrameSpeed = 10.f;

	return S_OK;
}

_uint CHellBossSpawnBall::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	// 스켸일
	m_pTransformCom->m_TransformDesc.vScale.x += 1.f * fDeltaTime;
	m_pTransformCom->m_TransformDesc.vScale.y += 1.f * fDeltaTime;

	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
		SpawnMonster();		// 몬스터 생성
	}

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHellBossSpawnBall::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossSpawnBall::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CHellBossSpawnBall::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

// 몬스터 생성
void CHellBossSpawnBall::SpawnMonster()
{
	// 몬스터 생성
	MonsterBasicArgument* _MonsterBasicArgument = new MonsterBasicArgument;;
	_MonsterBasicArgument->uiSize = sizeof(MonsterBasicArgument);
	_MonsterBasicArgument->pPlayer = static_cast<CPlayer*>(CManagement::Get_Instance()->GetGameObject(-1, L"Layer_Player"));
	_MonsterBasicArgument->vPosition = m_pTransformCom->m_TransformDesc.vPosition;
	m_pManagement->AddScheduledGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_Hellhound",
		L"Layer_Monster",
		nullptr, (void*)_MonsterBasicArgument);
}

HRESULT CHellBossSpawnBall::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 리틀 데몬 원형 총알 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Stormball",
		L"Com_Texture_HellBoss_Spawnball",
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

CHellBossSpawnBall* CHellBossSpawnBall::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossSpawnBall* pInstance = new CHellBossSpawnBall(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossSpawnBall");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossSpawnBall::Clone(void* pArg/* = nullptr*/)
{
	CHellBossSpawnBall* pClone = new CHellBossSpawnBall(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossSpawnBall");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossSpawnBall::Free()
{
	CBullet::Free();
}
