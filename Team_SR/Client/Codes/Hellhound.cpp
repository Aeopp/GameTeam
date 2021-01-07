#include "stdafx.h"
#include "..\Headers\Hellhound.h"


CHellhound::CHellhound(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
	, m_fCountdown(0.f), m_fNextAtkWait(0.f), m_fPlayerTrackingCount(0.f), m_fpAction(nullptr)
	, m_eAwareness(AWARENESS::End), m_ePhase(PHASE::End)
	, m_fpMonsterAI{}, isDamaged(false)
{
}


HRESULT CHellhound::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	GibTable = { 44,45,46,47,48,49,50,51};


	return S_OK;
}

HRESULT CHellhound::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 3.5f,3.5f,3.5f };

	// 몬스터 원본 스텟
	m_stOriginStatus.fHP = 80.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 20.f;
	m_stOriginStatus.fMeleeRange = 6.f;
	m_stOriginStatus.fDetectionRange = 50.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Com_Texture_Hellhound_EggHatch";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 12;
	m_fFrameSpeed = 10.f;


	// 부화
	m_fpAction = &CHellhound::Action_EggHatch;
	// 플레이어를 인식하지 못함
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Full] = &CHellhound::AI_NoAwareness;
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Half] = &CHellhound::AI_NoAwareness;

	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Full] = &CHellhound::AI_NormalPattern;		// 일반 패턴
	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Half] = &CHellhound::AI_DamagedPattern;		// 손상된 패턴

	return S_OK;
}

_uint CHellhound::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	// 테스트
	//if (GetAsyncKeyState('1') & 0x8000)
	//	Hit(nullptr, { {0.f, 0.f, 1.f}, 1.f });

	// 몬스터가 죽었음
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}
	if (LightHitTime > 0.0f)return 0;
	Update_AI(fDeltaTime);	// 업데이트 AI

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHellhound::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellhound::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;
	
	_CollisionComp->DebugDraw();

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(Set_Texture()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CHellhound::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 텍스처
	CTexture* pTexture;
	// 부화
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_EggHatch",
		L"Com_Texture_Hellhound_EggHatch",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_EggHatch", pTexture);

	// 대기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Idle",
		L"Com_Texture_Hellhound_Idle",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Idle", pTexture);

	// 근접 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Attack",
		L"Com_Texture_Hellhound_Attack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Attack", pTexture);

	// 달리기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Run",
		L"Com_Texture_Hellhound_Run",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Run", pTexture);

	// 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Hurt",
		L"Com_Texture_Hellhound_Hurt",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Hurt", pTexture);

	// 손상
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Damage",
		L"Com_Texture_Hellhound_Damage",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Damage", pTexture);

	// 손상 근접 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedAttack",
		L"Com_Texture_Hellhound_DamagedAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_DamagedAttack", pTexture);

	// 손상 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedHit",
		L"Com_Texture_Hellhound_DamagedHit",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_DamagedHit", pTexture);

	// 손상 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_DamagedWalk",
		L"Com_Texture_Hellhound_DamagedWalk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_DamagedWalk", pTexture);

	// 죽음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hellhound_Death",
		L"Com_Texture_Hellhound_Death",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Hellhound_Death", pTexture);

#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.75f;
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

// 몬스터가 피해를 받음
void CHellhound::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::HELLGROUND);
	CSoundMgr::Get_Instance()->PlaySound(L"hangman_pain1.wav", CSoundMgr::HELLGROUND);
	// 체력이 없음
	if (m_stStatus.fHP <= 0) {
		// 몬스터가 안죽었으면
		if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead))) {
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			bGravity = false;						// 중력 OFF
			m_fpAction = &CHellhound::Action_Dead;
			m_wstrTextureKey = L"Com_Texture_Hellhound_Death";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 12;
			m_fFrameSpeed = 10.f;
		}
		return;
	}

	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;

	// 플레이어 추적 ON
	m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::PlayerTracking);
	m_fPlayerTrackingCount = 20.f;

	// 텍스처 교체 불가
	if ((m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::TextureChangeLock))) {
		return;
	}

	if (!isDamaged) {
		// 체력이 50%
		if (m_stStatus.fHP <= m_stOriginStatus.fHP * 0.5f) {
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::TextureChangeLock);	// 텍스처 교체 락 ON
			m_fpAction = &CHellhound::Action_Damage;
			m_wstrTextureKey = L"Com_Texture_Hellhound_Damage";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 3;
			m_fFrameSpeed = 5.f;
			return;
		}

		// 피해를 받아서 현제 행동 취소
		// Hurt 텍스처를 취함
		m_fpAction = &CHellhound::Action_Hit;
		m_wstrTextureKey = L"Com_Texture_Hellhound_Hurt";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 1;
		m_fFrameSpeed = 5.f;
	}
	else {
		// 피해를 받아서 현제 행동 취소
		// Hit 텍스처를 취함
		m_fpAction = &CHellhound::Action_Hit;
		m_wstrTextureKey = L"Com_Texture_Hellhound_DamagedHit";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 1;
		m_fFrameSpeed = 5.f;
	}
}

void CHellhound::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	if (L"Floor" == _CollisionInfo.Flag)
	{
		bGravity = false;
		//m_pTransformCom->m_TransformDesc.vPosition.y = _CollisionInfo.IntersectPoint.y + 5;
	}
}

// AI는 하나의 행동을 끝마친 후에 새로운 행동을 받는다
void CHellhound::Update_AI(float fDeltaTime)
{
	// 다음 공격 대기 시간 감소
	m_fNextAtkWait -= fDeltaTime;
	// 플레이어 추적 시간 감소
	m_fPlayerTrackingCount -= fDeltaTime;
	if (m_fPlayerTrackingCount <= 0.f) {
		// 플레이어 추적 OFF
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::PlayerTracking);
	}

	// 몬스터 행동
	if (!(this->*m_fpAction)(fDeltaTime)) {
		return;
	}

	// 플레이어가 사정거리 안에 있는가?
	if (PlayerAwareness()) {
		// 플레이어 추적 ON
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::PlayerTracking);
		m_fPlayerTrackingCount = 20.f;
	}

	// 플레이어를 추적중인가?
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::PlayerTracking)) {
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

// 플레이어를 인식하지 못함
void CHellhound::AI_NoAwareness()
{
	m_fpAction = &CHellhound::Action_Idle;	// 대기
	m_fCountdown = 1.f;		// 1초
	// 텍스처 키, 프레임
	m_wstrTextureKey = L"Com_Texture_Hellhound_Idle";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
}

// 일반 패턴
void CHellhound::AI_NormalPattern()
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
	m_fpAction = &CHellhound::Action_Melee;
	m_wstrTextureKey = L"Com_Texture_Hellhound_Attack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 10;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellhound::Action_Move;
	m_fCountdown = 1.f;		// 1초 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_Hellhound_Run")) {
		m_wstrTextureKey = L"Com_Texture_Hellhound_Run";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 5;
		m_fFrameSpeed = 10.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

// 손상된 패턴
void CHellhound::AI_DamagedPattern()
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
	m_fpAction = &CHellhound::Action_Melee;
	m_wstrTextureKey = L"Com_Texture_Hellhound_DamagedAttack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 7;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellhound::Action_Move;
	m_fCountdown = 1.f;		// 1초 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_Hellhound_DamagedWalk")) {
		m_wstrTextureKey = L"Com_Texture_Hellhound_DamagedWalk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 6;
		m_fFrameSpeed = 10.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

bool CHellhound::Action_EggHatch(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_wstrTextureKey = L"Com_Texture_Hellhound_Idle";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 1;
		m_fFrameSpeed = 10.f;
		return true;
	}

	return false;
}

// 행동 대기
bool CHellhound::Action_Idle(float fDeltaTime)
{
	// 지정된 시간만큼 행동 대기
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}
	return false;
}

// 이동
bool CHellhound::Action_Move(float fDeltaTime)
{
	// 더 이상 이동할 좌표가 없으면 이동을 끝냄
	if (m_listMovePos.size() == 0) {
		return true;
	}

	// 리스트에 저장된 이동 좌표 하나 꺼냄
	list<vec3>::iterator iter = m_listMovePos.begin();
	vec3 vDestPos = *iter;

	// 방향
	vec3 vDir = vDestPos - m_pTransformCom->m_TransformDesc.vPosition;
	vDir.y = 0.f;
	// 길이
	float fLength = D3DXVec3Length(&vDir);
	// 리스트에서 꺼낸 좌표 까지 가까이 왔으면
	if (m_stStatus.fSpeed * fDeltaTime > fLength) {
		// 현재 좌표는 리스트에서 지움
		m_listMovePos.erase(iter);
	}


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

// 근접 공격
bool CHellhound::Action_Melee(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 1.f;
		CMonster::MeleeAttack();
		return true;
	}

	return false;
}

// 공격받아서 경직
bool CHellhound::Action_Hit(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		return true;
	}
	return false;
}

// 죽음
bool CHellhound::Action_Dead(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Dead);	// 몬스터가 죽었어요
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		return false;
	}

	return false;
}

bool CHellhound::Action_Damage(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		isDamaged = true;	// 손상 상태 ON
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::TextureChangeLock); // 텍스처 교체 락 OFF
		m_stStatus.fSpeed = 8.f;		// 스피드 대폭 감소
		return true;
	}
	return false;
}

CHellhound* CHellhound::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellhound* pInstance = new CHellhound(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellhound");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellhound::Clone(void* pArg/* = nullptr*/)
{
	CHellhound* pClone = new CHellhound(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellhound");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellhound::Free()
{
	CMonster::Free();
}
