#include "stdafx.h"
#include "..\Headers\Hangman.h"


CHangman::CHangman(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
	, m_fCountdown(0.f), m_fNextAtkWait(0.f), m_fpAction(nullptr)
	, m_eAwareness(AWARENESS::End), m_ePhase(PHASE::End)
	, m_fpMonsterAI{}, isDamaged(false)
{
}


HRESULT CHangman::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHangman::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 몬스터 원본 스텟
	m_stOriginStatus.fHP = 200.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fMeleeRange = 6.f;
	m_stOriginStatus.fDetectionRange = 50.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Com_Texture_Hangman_Back";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	m_fFrameSpeed = 10.f;

	// 기본 대기 행동
	m_fpAction = &CHangman::Action_Idle;
	// 플레이어를 인식하지 못함
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Full] = &CHangman::AI_NoAwareness;
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Half] = &CHangman::AI_NoAwareness;

	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Full] = &CHangman::AI_NormalPattern;		// 일반 패턴
	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Half] = &CHangman::AI_DamagedPattern;		// 손상된 패턴

	return S_OK;
}

_uint CHangman::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	// 테스트
	//if (GetAsyncKeyState('1') & 0x8000)
	//	Hit(nullptr, { {0.f, 0.f, 1.f}, 1.f });

	// 몬스터가 죽었음
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}

	Update_AI(fDeltaTime);	// 업데이트 AI

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHangman::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHangman::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(Set_Texture()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHangman::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 텍스처
	CTexture* pTexture;
	// 뒤돌아 대기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Back",
		L"Com_Texture_Hangman_Back",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Back", pTexture);

	// 원거리 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Bomb_Attack",
		L"Com_Texture_Hangman_Bomb_Attack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Bomb_Attack", pTexture);

	// 근접 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Attack",
		L"Com_Texture_Hangman_Attack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Attack", pTexture);

	// 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Walk",
		L"Com_Texture_Hangman_Walk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Walk", pTexture);

	// 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Hit",
		L"Com_Texture_Hangman_Hit",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Hit", pTexture);

	// 손상
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Damage",
		L"Com_Texture_Hangman_Damage",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Damage", pTexture);

	// 손상 근거리 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamageAttack",
		L"Com_Texture_Hangman_DamageAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_DamageAttack", pTexture);

	// 손상 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamagedHit",
		L"Com_Texture_Hangman_DamagedHit",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_DamagedHit", pTexture);

	// 손상 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_DamagedWalk",
		L"Com_Texture_Hangman_DamagedWalk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_DamagedWalk", pTexture);

	// 죽음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Death",
		L"Com_Texture_Hangman_Death",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hangman_Death", pTexture);

#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 2.5f;
	_Info.Tag = CCollisionComponent::ETag::Monster;
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

// 몬스터가 피해를 받음
void CHangman::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소

	// 체력이 없음
	if (m_stStatus.fHP <= 0) {
		// 몬스터가 안죽었으면
		if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead))) {
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			m_fpAction = &CHangman::Action_Dead;
			m_wstrTextureKey = L"Com_Texture_Hangman_Death";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 10;
			m_fFrameSpeed = 10.f;
		}
		return;
	}

	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;

	// 텍스처 교체 불가
	if ((m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::TextureChangeLock))) {
		return;
	}

	if (!isDamaged) {
		// 체력이 50%
		if (m_stStatus.fHP <= m_stOriginStatus.fHP * 0.5f) {
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::TextureChangeLock);	// 텍스처 교체 락 ON
			m_fpAction = &CHangman::Action_Damage;
			m_wstrTextureKey = L"Com_Texture_Hangman_Damage";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 6;
			m_fFrameSpeed = 5.f;
			return;
		}

		// 피해를 받아서 현제 행동 취소
		// Hit 텍스처를 취함
		m_fpAction = &CHangman::Action_Hit;
		m_wstrTextureKey = L"Com_Texture_Hangman_Hit";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 1;
		m_fFrameSpeed = 5.f;
	}
	else {
		// 피해를 받아서 현제 행동 취소
		// Hit 텍스처를 취함
		m_fpAction = &CHangman::Action_Hit;
		m_wstrTextureKey = L"Com_Texture_Hangman_DamagedHit";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 1;
		m_fFrameSpeed = 5.f;
	}
}

// AI는 하나의 행동을 끝마친 후에 새로운 행동을 받는다
void CHangman::Update_AI(float fDeltaTime)
{
	// 다음 공격 대기 시간 감소
	m_fNextAtkWait -= fDeltaTime;

	// 몬스터 행동
	if (!(this->*m_fpAction)(fDeltaTime)) {
		return;
	}

	// 플레이어를 인식했는가?
	if (PlayerAwareness()) {
		m_eAwareness = AWARENESS::Yes;	// 플레이어 발견
	}
	else {
		m_eAwareness = AWARENESS::No;	// 플레이어 몰라
	}

	// 체력에 따른 페이즈
	if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.5f) {
		m_ePhase = PHASE::HP_Full;	// 체력이 절반 이상
	}
	else {
		m_ePhase = PHASE::HP_Half;	// 체력이 절반 이하
	}

	// AI 처리
	(this->*m_fpMonsterAI[(int)m_eAwareness][(int)m_ePhase])();
}

HRESULT CHangman::Set_Texture()
{
	// 텍스처 찾기
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

// 플레이어를 인식하지 못함
void CHangman::AI_NoAwareness()
{
	m_fpAction = &CHangman::Action_Idle;	// 대기
	m_fCountdown = 1.f;		// 1초
	// 텍스처 키, 프레임
	m_wstrTextureKey = L"Com_Texture_Hangman_Back";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
}

// 일반 패턴
void CHangman::AI_NormalPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		// 플레이어가 가까이 근접해 있나
		if (PlayerBeNear()) {
			// 근접 공격
			goto RETURN_MELEE;
		}
		else {
			// 원거리 공격
			goto RETURN_SHOOT;
		}
	}
	else {
		// 플레이어가 멀리 있나
		if (!PlayerBeNear()) {
			goto RETURN_MOVE;
		}
		goto RETURN_MELEE;
	}

RETURN_MELEE:	// 근접 공격
	m_fpAction = &CHangman::Action_Melee;
	m_wstrTextureKey = L"Com_Texture_Hangman_Attack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 13;
	m_fFrameSpeed = 10.f;
	return;

RETURN_SHOOT:	// 원거리 공격
	m_fpAction = &CHangman::Action_Shoot;
	m_wstrTextureKey = L"Com_Texture_Hangman_Bomb_Attack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 15;
	m_fFrameSpeed = 10.f;
	// 목표 = 플레이어 위치 - 몬스터 위치
	m_vAim = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	m_vAim.y = 0.f;
	D3DXVec3Normalize(&m_vAim, &m_vAim);
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHangman::Action_Move;
	m_fCountdown = 1.f;		// 1초 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_Hangman_Walk")) {
		m_wstrTextureKey = L"Com_Texture_Hangman_Walk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 6;
		m_fFrameSpeed = 10.f;
	}
	return;
}

// 손상된 패턴
void CHangman::AI_DamagedPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		// 플레이어가 가까이 근접해 있나
		if (PlayerBeNear()) {
			// 근접 공격
			goto RETURN_MELEE;
		}
	}
	// 플레이어가 멀리 있나
	if (!PlayerBeNear()) {
		goto RETURN_MOVE;
	}

RETURN_MELEE:	// 근접 공격
	m_fpAction = &CHangman::Action_Melee;
	m_wstrTextureKey = L"Com_Texture_Hangman_DamageAttack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 10;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHangman::Action_Move;
	m_fCountdown = 1.f;		// 1초 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_Hangman_DamagedWalk")) {
		m_wstrTextureKey = L"Com_Texture_Hangman_DamagedWalk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 6;
		m_fFrameSpeed = 10.f;
	}
	return;
}

// 행동 대기
bool CHangman::Action_Idle(float fDeltaTime)
{
	// 지정된 시간만큼 행동 대기
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}
	return false;
}

// 이동
bool CHangman::Action_Move(float fDeltaTime)
{
	// 방향
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	// 포지션 이동
	m_pTransformCom->m_TransformDesc.vPosition += vDir * m_stStatus.fSpeed * fDeltaTime;

	// 지정된 시간만큼 이동 후 정지
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}

	// 플레어에게 가까이 접근했으면 정지
	if (PlayerBeNear()) {
		return true;
	}

	return false;
}

// 원거리 공격
bool CHangman::Action_Shoot(float fDeltaTime)
{
	// 단발 쏴
	if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Shoot)) && m_fFrameCnt >= 11.f) {	// 3번째 텍스처 때 쏨
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Shoot);
		BulletBasicArgument* pArg = new BulletBasicArgument;
		pArg->uiSize = sizeof(BulletBasicArgument);
		pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition;	// 생성 위치
		pArg->vDir = m_vAim;	// 방향
		m_pManagement->AddScheduledGameObjectInLayer(
			(_int)ESceneID::Static,
			L"GameObject_HangmanBomb",
			L"Layer_HangmanBomb",
			nullptr, (void*)pArg);
	}

	if (m_bFrameLoopCheck) {
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::Shoot);
		m_fNextAtkWait = 5.f;
		return true;
	}

	return false;
}

// 근접 공격
bool CHangman::Action_Melee(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 1.f;
		return true;
	}

	return false;
}

// 공격받아서 경직
bool CHangman::Action_Hit(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		return true;
	}
	return false;
}

// 죽음
bool CHangman::Action_Dead(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag != static_cast<BYTE>(MonsterFlag::Dead);	// 몬스터가 죽었어요
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		return false;
	}

	return false;
}

bool CHangman::Action_Damage(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		isDamaged = true;	// 손상 상태 ON
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::TextureChangeLock); // 텍스처 교체 락 OFF
		m_stStatus.fSpeed = 12.f;		// 속도 약간 증가
		return true;
	}
	return false;
}

CHangman* CHangman::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHangman* pInstance = new CHangman(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHangman");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHangman::Clone(void* pArg/* = nullptr*/)
{
	CHangman* pClone = new CHangman(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHangman");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHangman::Free()
{
	CMonster::Free();
}
