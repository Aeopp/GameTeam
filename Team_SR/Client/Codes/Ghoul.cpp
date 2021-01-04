#include "stdafx.h"
#include "..\Headers\Ghoul.h"


CGhoul::CGhoul(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
	, m_fCountdown(0.f), m_fNextAtkWait(0.f), m_fPlayerTrackCount(0.f), m_fpAction(nullptr)
	, m_eAwareness(AWARENESS::End)
	, m_fpMonsterAI{}, isHide(true)
{
}


HRESULT CGhoul::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhoul::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 3.5f,3.5f,3.5f };

	// 몬스터 원본 스텟
	m_stOriginStatus.fHP = 10.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 3.f;
	m_stOriginStatus.fMeleeRange = 12.f;
	m_stOriginStatus.fDetectionRange = 12.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Com_Texture_Ghoul_Hide";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	m_fFrameSpeed = 10.f;

	m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON

	// 대기
	m_fpAction = &CGhoul::Action_Idle;
	// 플레이어를 인식하지 못함
	m_fpMonsterAI[(int)AWARENESS::No] = &CGhoul::AI_NoAwareness;
	m_fpMonsterAI[(int)AWARENESS::Yes] = &CGhoul::AI_NormalPattern;		// 일반 패턴

	return S_OK;
}

_uint CGhoul::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	// 몬스터가 죽었음
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}

	Update_AI(fDeltaTime);	// 업데이트 AI

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CGhoul::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CGhoul::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	_CollisionComp->DebugDraw();

	return S_OK;
}

HRESULT CGhoul::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 텍스처
	CTexture* pTexture;
	// 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Attack",
		L"Com_Texture_Ghoul_Attack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Attack", pTexture);

	// 뒤돌아봄
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Back",
		L"Com_Texture_Ghoul_Back",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Back", pTexture);

	// 죽음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Death",
		L"Com_Texture_Ghoul_Death",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Death", pTexture);

	// 땅파기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_DigOut",
		L"Com_Texture_Ghoul_DigOut",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_DigOut", pTexture);

	// 숨음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Hide",
		L"Com_Texture_Ghoul_Hide",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Hide", pTexture);

	// 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Hit",
		L"Com_Texture_Ghoul_Hit",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Hit", pTexture);

	// 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ghoul_Walk",
		L"Com_Texture_Ghoul_Walk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_Ghoul_Walk", pTexture);

#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = m_pTransformCom->m_TransformDesc.vScale.y + 1.f;
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
void CGhoul::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소
	//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::HELLGROUND);
	//CSoundMgr::Get_Instance()->PlaySound(L"hangman_pain1.wav", CSoundMgr::HELLGROUND);
	// 체력이 없음
	if (m_stStatus.fHP <= 0) {
		// 몬스터가 안죽었으면
		if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead))) {
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			bGravity = false;						// 중력 OFF
			m_fpAction = &CGhoul::Action_Dead;
			m_wstrTextureKey = L"Com_Texture_Ghoul_Death";
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
	m_fPlayerTrackCount = 7.f;

	// 텍스처 교체 불가
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::TextureChangeLock)) {
		return;
	}

	// 피해를 받아서 현제 행동 취소
	// Hit 텍스처를 취함
	m_fpAction = &CGhoul::Action_Hit;
	m_wstrTextureKey = L"Com_Texture_Ghoul_Hit";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	m_fFrameSpeed = 5.f;
}

// AI는 하나의 행동을 끝마친 후에 새로운 행동을 받는다
void CGhoul::Update_AI(float fDeltaTime)
{
	// 다음 공격 대기 시간 감소
	m_fNextAtkWait -= fDeltaTime;
	// 플레이어 추적 시간 감소
	m_fPlayerTrackCount -= fDeltaTime;
	if (m_fPlayerTrackCount <= 0.f) {
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
		m_fPlayerTrackCount = 5.f;
	}

	// 플레이어를 추적중인가?
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::PlayerTracking)) {
		m_eAwareness = AWARENESS::Yes;	// 플레이어 발견
	}
	else {
		m_eAwareness = AWARENESS::No;	// 플레이어 몰라
	}

	// AI 처리
	(this->*m_fpMonsterAI[(int)m_eAwareness])();
}

// 플레이어를 인식하지 못함
void CGhoul::AI_NoAwareness()
{
	// 숨어있음
	if (isHide) {
		// 숨음
		goto RETURN_HIDE;
	}
	else {
		// 대기
		goto RETURN_IDLE;
	}

RETURN_HIDE:	// 숨음
	m_fpAction = &CGhoul::Action_Idle;	// 대기
	m_fCountdown = 1.f;		// 1초
	m_wstrTextureKey = L"Com_Texture_Ghoul_Hide";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	return;

RETURN_IDLE:	// 대기
	m_fpAction = &CGhoul::Action_Idle;	// 대기
	m_fCountdown = 1.f;		// 1초
	m_wstrTextureKey = L"Com_Texture_Ghoul_Back";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	return;
}

// 일반 패턴
void CGhoul::AI_NormalPattern()
{
	// 숨어있음
	if (isHide) {
		// 플레이어가 가까이 근접해 있나
		if (PlayerBeNear()) {
			// 땅파고 나옴
			goto RETURN_DIG_OUT;
		}
	}

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
	m_fpAction = &CGhoul::Action_Melee;
	m_wstrTextureKey = L"Com_Texture_Ghoul_Attack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 15;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CGhoul::Action_Move;
	m_fCountdown = 1.f;		// 1초 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_Ghoul_Walk")) {
		m_wstrTextureKey = L"Com_Texture_Ghoul_Walk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 6;
		m_fFrameSpeed = 5.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;

RETURN_DIG_OUT:	// 땅파고 나옴
	m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::TextureChangeLock);	// 텍스처 교체 락 ON
	m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::HPLock);				// HP 락 OFF
	m_fpAction = &CGhoul::Action_DigOut;
	m_wstrTextureKey = L"Com_Texture_Ghoul_DigOut";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 16;
	isHide = false;	// 숨은 상태 아님
	m_stStatus.fDetectionRange = 20.f;
	m_stOriginStatus.fDetectionRange = 20.f;
	return;
}

// 행동 대기
bool CGhoul::Action_Idle(float fDeltaTime)
{
	// 지정된 시간만큼 행동 대기
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}
	return false;
}

bool CGhoul::Action_DigOut(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::TextureChangeLock); // 텍스처 교체 락 OFF
		return true;
	}
	return false;
}

// 이동
bool CGhoul::Action_Move(float fDeltaTime)
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
bool CGhoul::Action_Melee(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 1.f;
		CMonster::MeleeAttack();
		return true;
	}

	return false;
}

// 공격받아서 경직
bool CGhoul::Action_Hit(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		return true;
	}
	return false;
}

// 죽음
bool CGhoul::Action_Dead(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Dead);	// 몬스터가 죽었어요
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		return false;
	}

	return false;
}

CGhoul* CGhoul::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGhoul* pInstance = new CGhoul(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGhoul");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CGhoul::Clone(void* pArg/* = nullptr*/)
{
	CGhoul* pClone = new CGhoul(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGhoul");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGhoul::Free()
{
	CMonster::Free();
}
