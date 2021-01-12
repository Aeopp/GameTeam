#include "stdafx.h"
#include "..\Headers\Glacier.h"
#include "CollisionComponent.h"
#include "Camera.h"
#include "GlacierBullet.h"
#include "GlacierParticle.h"
CGlacier::CGlacier(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}


HRESULT CGlacier::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CGlacier>();

	GibTable = { 16,17,18,19,20,16,17,18,19,20};

	return S_OK;
}

HRESULT CGlacier::ReadyGameObject(void* pArg /*= nulptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = m_wstrBase + L"Move";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 15;
	
	//bGravity = false;

	m_stOriginStatus.fHP = 80.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fDetectionRange = 50.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	m_pTransformCom->m_TransformDesc.vScale = {2.5f,2.5f,2.5f };


	m_fpAction = &CGlacier::Action_Idle;
	// 플레이어를 인식하지 못함
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_High] = &CGlacier::AI_NoAwareness;
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_Half] = &CGlacier::AI_NoAwareness;
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_Low]  = &CGlacier::AI_NoAwareness;
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_ZERO] = &CGlacier::AI_NoAwareness;


	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_High] = &CGlacier::AI_FirstPhase;		// 적극적으로 공격
	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_Half] = &CGlacier::AI_SecondPhase;	// 소극적으로 공격
	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_Low] = &CGlacier::AI_ThirdPhase;	// 소극적으로 공격
	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_ZERO] = &CGlacier::AI_DeadPhase;	// 소극적으로 공격
	return S_OK;
}

_uint CGlacier::UpdateGameObject(float fDeltaTime)
{
	// 2020.12.17 10:06 KMJ
	// 죽으면 빌보드 처리만 함
	//if (true == m_bDead)
	//	return 0;
	CMonster::UpdateGameObject(fDeltaTime);

	//테스트
	//if (GetAsyncKeyState('4') & 0x8000)
	//	m_stStatus.fHP -= 1;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GALICER);
	CSoundMgr::Get_Instance()->PlaySound(L"vaseBreak.wav", CSoundMgr::GALICER);

	
	// 2020.12.17 10:06 KMJ
	// 몬스터가 죽음, AI, 충돌처리 X
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}
	if (LightHitTime > 0.0f)return 0;

	//테스트
	Update_AI(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CGlacier::LateUpdateGameObject(float fDeltaTime)
{
	// 2020.12.17 10:09 KMJ
	// bool 값으로 분기를 나누는 것보다
	// Action_Death 함수에서 프레임을 마지막 프레임만 반복하게 하는게 나을것 같음
	//if (true == m_bDead)
	//{
	//	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
	//		return 0;
	//	return 0;
	//}
	CMonster::LateUpdateGameObject(fDeltaTime);
	FreezeGlacierParticleTime -= fDeltaTime;

	// Frame_Move, AddGameObjectInRenderer 순서
	// m_bFrameLoopCheck 불값 관련해서 체크하는 처리가 있는데 
	// 장치에 텍스처 초기화 예로들면 end -> 다시 start 프레임으로 넘어가고 난 후에
	// m_bFrameLoopCheck true 되서 다음 Update에서 순간적으로 시작 프레임이 보일 것 같음
	//if (AWARENESS::Yes == m_eAwareness)
	//	m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CGlacier::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(Set_Texture()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

// 몬스터가 피해를 받음
void CGlacier::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소

	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;
	CreateParticle();
}

void CGlacier::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	if (L"Floor" == _CollisionInfo.Flag)
	{
		bGravity = false;
	}
}

void CGlacier::ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)
{	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::ParticleHit(_Particle, _CollisionInfo);		// CMonster 에서 HP 감소

	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;
	CreateParticle();
}

HRESULT CGlacier::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	CTexture* pTexture;
	// Move
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Move",
		L"Com_Texture_GlacierMove",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Move", pTexture);
	// Death
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Death",
		L"Com_Texture_GlacierDeath",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Death", pTexture);
	// Attack
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Attack",
		L"Com_Texture_GlacierAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Attack", pTexture);
	// Hurt
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Hurt",
		L"Com_Texture_GlacierHurt",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Hurt", pTexture);

	// Collision
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.25f;
	_Info.Tag = CCollisionComponent::ETag::Monster;
	_Info.bFloorCollision = true;
	_Info.bWallCollision = true;
	_Info.Owner = this;

	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);
	

	return S_OK;
}

HRESULT CGlacier::Set_Texture()
{
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}




void CGlacier::Update_AI(float fDeltaTime)
{
	if ((this->*m_fpAction)(fDeltaTime)) 
	{
		// 플레이어를 인식했는가?
		if (PlayerAwareness()) {
			m_eAwareness = AWARENESS::Yes;	
		}
		else {
			m_eAwareness = AWARENESS::No;	
		}


		if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.7f) {
			m_ePhase = PHASE::HP_High;	
		}
		else if(m_stStatus.fHP < m_stOriginStatus.fHP * 0.7f
			&& m_stStatus.fHP > m_stOriginStatus.fHP * 0.4f)
		{
			m_ePhase = PHASE::HP_Half;	
		}
		else if(m_stStatus.fHP < m_stOriginStatus.fHP * 0.4f
			&& m_stStatus.fHP > 0)
		{
			m_ePhase = PHASE::HP_Low;
		}
		else if (m_stStatus.fHP < 0)
		{
			m_ePhase = PHASE::HP_ZERO;
		}

		(this->*m_fpGlacierAI[(int)m_eAwareness][(int)m_ePhase])();
	}
}

void CGlacier::AI_NoAwareness()
{
	m_fpAction = &CGlacier::Action_Idle;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Move";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 0.f;
}

void CGlacier::AI_FirstPhase()
{
	m_fpAction = &CGlacier::Action_Move;
	m_fCountDown = 0.5f;
	m_wstrTextureKey = m_wstrBase + L"Move";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 15.f;
}

void CGlacier::AI_SecondPhase()
{
	//총알 발사

	m_fpAction = &CGlacier::Action_Hurt;
	m_fCountDown = 0.5f;
	m_wstrTextureKey = m_wstrBase + L"Hurt";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 15.f;

}

void CGlacier::AI_ThirdPhase()
{
	//총알 발사
	m_fpAction = &CGlacier::Action_Shoot;
	m_fCountDown = 0.5f;
	m_wstrTextureKey = m_wstrBase + L"Attack";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 10.f;
}

void CGlacier::AI_DeadPhase()
{
	// 2020.12.17 10:02 KMJ
	m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락
	m_fpAction = &CGlacier::Action_Death;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Death";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 8.f;
}

bool CGlacier::Action_Move(float fDeltaTime)
{
	const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	const _vector vGlacierPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vLook = vPlayerPos - vGlacierPos;
	vLook.y = 0.f;
	float fLookLength = D3DXVec3Length(&vLook);
	D3DXVec3Normalize(&vLook, &vLook);


	//if (fLookLength < 10)
	//{
	//	m_stStatus.fHP = 40.f;
	//	return true;
	//}
	if(fLookLength > 10)
		m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * m_stStatus.fSpeed;

	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}

	return false;
}

bool CGlacier::Action_Idle(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0) 
	{
		return true;
	}
	return false;
}

bool CGlacier::Action_Shoot(float fDeltaTime)
{
	//m_fCountDown -= fDeltaTime;
	//if (m_fCountDown <= 0)
	//{
	//	return true;
	//}

	if (m_bFrameLoopCheck)
	{
		CreateBullet();
		return true;
	}

	return false;
}

bool CGlacier::Action_Hurt(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CGlacier::Action_Death(float fDeltaTime)
{
	if (m_bFrameLoopCheck)
	{
		// 2020.12.17 10:03 KMJ
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Dead);	// 몬스터가 죽었어요
		//m_bDead = true;
		//m_fFrameCnt = 8.f;
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;

		for (_uint i = 0; i < 10; ++i)
			CreateParticle();
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GALICER);
		CSoundMgr::Get_Instance()->PlaySound(L"ice_breaking_1.wav", CSoundMgr::GALICER);
	}
	return false;
}

void CGlacier::CreateBullet()
{
	// 2020.12.16 17:35 KMJ
	// 예약된 생성은 아규먼트가 다음 예약처리 시점까지 메모리에 있어야 함
	// 동적 생성해서 힙에 데이터를 남김
	// 메모리 해제는 만들어지는 객체에서 아규먼트 데이터 처리후 해제
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GALICER);
	CSoundMgr::Get_Instance()->PlaySound(L"supersonicnew_new_rocket_shot_.wav", CSoundMgr::GALICER);
	m_vAim = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	BulletBasicArgument* pArg = new BulletBasicArgument;
	pArg->uiSize = sizeof(BulletBasicArgument);
	pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition;	// 생성 위치
	pArg->vDir = m_vAim;	// 방향
	m_pManagement->AddScheduledGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacierBullet>(),
		L"Layer_" + TYPE_NAME<CGlacierBullet>(),
		nullptr, (void*)pArg);
}

void CGlacier::CreateParticle()
{
	m_pManagement->AddScheduledGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacierParticle>(),
		L"Layer_" + TYPE_NAME<CGlacierParticle>(),
		nullptr, (void*)&m_pTransformCom->m_TransformDesc.vPosition);
}



CGlacier* CGlacier::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacier* pInstance = new CGlacier(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacier");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CGlacier::Clone(void* pArg/* = nullptr*/)
{
	CGlacier* pClone = new CGlacier(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacier::Free()
{
	// 2020.12.17 11:26 KMJ
	// CMonster 에서
	//SafeRelease(_CollisionComp);

	CMonster::Free();
}

void CGlacier::FreezeHit()
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::FreezeHit();		// CMonster 에서 HP 감소
	// 충돌 관련 정보
	if (FreezeGlacierParticleTime < 0.0f)
	{
		FreezeGlacierParticleTime = 0.7f;
		CreateParticle();
	}
}
