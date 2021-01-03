#include "stdafx.h"
#include "..\Headers\HellBoss.h"


CHellBoss::CHellBoss(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
	, m_fCountdown(0.f), m_fNextAtkWait(0.f), m_fPlayerTrackCount(0.f), m_iRepeatCount(0), m_fpAction(nullptr)
	, m_ePhase(PHASE::End)
	, m_fpMonsterAI{}
{
}


HRESULT CHellBoss::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBoss::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 5.f,5.f,5.f };

	// 몬스터 원본 스텟
	m_stOriginStatus.fHP = 100.f;//500.f;
	m_stOriginStatus.fATK = 20.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 7.f;
	m_stOriginStatus.fMeleeRange = 6.f;
	m_stOriginStatus.fDetectionRange = 50.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Com_Texture_LittleDemon_Idle";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	m_fFrameSpeed = 10.f;

	// 대기
	m_fpAction = &CHellBoss::Action_Idle;
	m_fCountdown = 0.5f;		// 0.5초
	m_ePhase = PHASE::LittleDemon;

	m_fpMonsterAI[(int)PHASE::LittleDemon] = &CHellBoss::AI_LittleDemonPattern;				// 리틀 데몬 패턴
	m_fpMonsterAI[(int)PHASE::TurboSatan] = &CHellBoss::AI_TurboSatanPattern;				// 터보 사탄 패턴
	m_fpMonsterAI[(int)PHASE::InjuredTurboSatan] = &CHellBoss::AI_InjuredTurboSatanPattern;	// 부상당한 터보 사탄 패턴
	m_fpMonsterAI[(int)PHASE::CacoDevil] = &CHellBoss::AI_CacoDevilPattern;					// 카코 데빌 패턴
	m_fpMonsterAI[(int)PHASE::FallenLord] = &CHellBoss::AI_FallenLordPattern;				// 몰락한 군주 패턴

	return S_OK;
}

_uint CHellBoss::UpdateGameObject(float fDeltaTime)
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

_uint CHellBoss::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBoss::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	_CollisionComp->DebugDraw();

	return S_OK;
}

HRESULT CHellBoss::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 텍스처
	CTexture* pTexture;
	//--------------------------------
	// 리틀 데몬
	//--------------------------------
	// 리틀 데몬 눈깔 빔
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_EyeBlast",
		L"Com_Texture_LittleDemon_EyeBlast",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_LittleDemon_EyeBlast", pTexture);

	// 리틀 데몬 대기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_Idle",
		L"Com_Texture_LittleDemon_Idle",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_LittleDemon_Idle", pTexture);

	// 리틀 데몬 변신
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_Morph",
		L"Com_Texture_LittleDemon_Morph",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_LittleDemon_Morph", pTexture);

	// 리틀 데몬 원거리 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_Shoot",
		L"Com_Texture_LittleDemon_Shoot",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_LittleDemon_Shoot", pTexture);

	// 리틀 데몬 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_LittleDemon_Walk",
		L"Com_Texture_LittleDemon_Walk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_LittleDemon_Walk", pTexture);

	//--------------------------------
	// 터보 사탄
	//--------------------------------
	// 터보 사탄 원거리 공격 끝
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_AttackEnd",
		L"Com_Texture_TurboSatan_AttackEnd",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_AttackEnd", pTexture);

	// 터보 사탄 원거리 공격 발싸
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_AttackFire",
		L"Com_Texture_TurboSatan_AttackFire",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_AttackFire", pTexture);

	// 터보 사탄 원거리 공격 회전
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_AttackSpin",
		L"Com_Texture_TurboSatan_AttackSpin",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_AttackSpin", pTexture);

	// 터보 사탄 원거리 공격 시작
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_AttackStart",
		L"Com_Texture_TurboSatan_AttackStart",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_AttackStart", pTexture);

	// 터보 사탄 손상
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_Damage",
		L"Com_Texture_TurboSatan_Damage",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_Damage", pTexture);

	// 터보 사탄 손상 원거리 로켓 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_DamagedAttack",
		L"Com_Texture_TurboSatan_DamagedAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_DamagedAttack", pTexture);

	// 터보 사탄 손상 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_DamagedWalk",
		L"Com_Texture_TurboSatan_DamagedWalk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_DamagedWalk", pTexture);

	// 터보 사탄 변신
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_Morph",
		L"Com_Texture_TurboSatan_Morph",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_Morph", pTexture);

	// 터보 사탄 이동
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_Walk",
		L"Com_Texture_TurboSatan_Walk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_TurboSatan_Walk", pTexture);

	//--------------------------------
	// 카코 데빌
	//--------------------------------
	// 카코 데빌 몬스터 소환
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_CacoDevil_Attack",
		L"Com_Texture_CacoDevil_Attack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_CacoDevil_Attack", pTexture);

	// 카코 데빌 눈깔 레이저
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_CacoDevil_EyeLasers",
		L"Com_Texture_CacoDevil_EyeLasers",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_CacoDevil_EyeLasers", pTexture);

	// 카코 데빌 부유
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_CacoDevil_Float",
		L"Com_Texture_CacoDevil_Float",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_CacoDevil_Float", pTexture);

	// 카코 데빌 변신
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_CacoDevil_Morph",
		L"Com_Texture_CacoDevil_Morph",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_CacoDevil_Morph", pTexture);

	// 카코 데빌 충격파
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_CacoDevil_Nova",
		L"Com_Texture_CacoDevil_Nova",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_CacoDevil_Nova", pTexture);

	//--------------------------------
	// 몰락한 군주
	//--------------------------------
	// 몰락한 군주 죽음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_FallenLord_Death",
		L"Com_Texture_FallenLord_Death",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_FallenLord_Death", pTexture);

	// 몰락한 군주 대기
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_FallenLord_Idle",
		L"Com_Texture_FallenLord_Idle",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Com_Texture_FallenLord_Idle", pTexture);

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
void CHellBoss::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소
	// 체력이 없음
	if (m_stStatus.fHP <= 0) {

		switch (m_ePhase)
		{
			// 리틀 데몬
		case CHellBoss::PHASE::LittleDemon:
			m_fpAction = &CHellBoss::Action_Morph;
			m_wstrTextureKey = L"Com_Texture_LittleDemon_Morph";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 31;
			m_fFrameSpeed = 10.f;

			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			//_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			m_ePhase = PHASE::TurboSatan;	// 페이즈 전환
			m_stOriginStatus.fSpeed = 8.f;
			m_stOriginStatus.fMeleeRange = 8.f;
			// 페이즈 전환되면서 몬스터 스텟이 바뀜
			m_stStatus = m_stOriginStatus;
			break;
			// 터보 사탄
		case CHellBoss::PHASE::TurboSatan:
			m_fpAction = &CHellBoss::Action_Morph;
			m_wstrTextureKey = L"Com_Texture_TurboSatan_Damage";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 6;
			m_fFrameSpeed = 10.f;

			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			//_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			m_ePhase = PHASE::InjuredTurboSatan;	// 페이즈 전환
			m_stStatus.fHP = m_stOriginStatus.fHP;	// 체력 회복
			break;
			// 부상당한 터보 사탄
		case CHellBoss::PHASE::InjuredTurboSatan:
			m_fpAction = &CHellBoss::Action_Morph;
			m_wstrTextureKey = L"Com_Texture_TurboSatan_Morph";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 65;
			m_fFrameSpeed = 10.f;

			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			//->bCollision = false;		// 충돌 처리 OFF
			m_ePhase = PHASE::CacoDevil;	// 페이즈 전환
			m_stOriginStatus.fSpeed = 9.f;
			m_stOriginStatus.fMeleeRange = 10.f;
			m_stOriginStatus.fDetectionRange = 40.f;
			// 페이즈 전환되면서 몬스터 스텟이 바뀜
			m_stStatus = m_stOriginStatus;
			break;
			// 카코 데빌
		case CHellBoss::PHASE::CacoDevil:
			m_fpAction = &CHellBoss::Action_LastMorph;		// 최종 페이즈 변신
			m_wstrTextureKey = L"Com_Texture_CacoDevil_Morph";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 43;
			m_fFrameSpeed = 10.f;

			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			//_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			m_ePhase = PHASE::FallenLord;	// 페이즈 전환
			m_stOriginStatus.fSpeed = 9.f;
			m_stOriginStatus.fMeleeRange = 50.f;
			m_stOriginStatus.fDetectionRange = 70.f;
			// 페이즈 전환되면서 몬스터 스텟이 바뀜
			m_stStatus = m_stOriginStatus;
			break;
			// 몰락한 군주
		case CHellBoss::PHASE::FallenLord:
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
			_CollisionComp->bCollision = false;		// 충돌 처리 OFF
			bGravity = false;						// 중력 OFF
			m_fpAction = &CHellBoss::Action_Dead;
			m_wstrTextureKey = L"Com_Texture_FallenLord_Death";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 26;
			m_fFrameSpeed = 10.f;
			break;
		default:
			// 예기치 못한 상황
			throw;
		}

		// 몬스터가 안죽었으면
		//if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead))) {
		//	m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);	// HP 락 ON
		//	_CollisionComp->bCollision = false;		// 충돌 처리 OFF
		//	bGravity = false;						// 중력 OFF
		//	m_fpAction = &CHellBoss::Action_Dead;
		//	m_wstrTextureKey = L"Com_Texture_Ghoul_Death";
		//	m_fFrameCnt = 0;
		//	m_fStartFrame = 0;
		//	m_fEndFrame = 12;
		//	m_fFrameSpeed = 10.f;
		//}
		return;
	}

	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;
}

// AI는 하나의 행동을 끝마친 후에 새로운 행동을 받는다
void CHellBoss::Update_AI(float fDeltaTime)
{
	// 다음 공격 대기 시간 감소
	m_fNextAtkWait -= fDeltaTime;

	// 몬스터 행동
	if (!(this->*m_fpAction)(fDeltaTime)) {
		return;
	}

	// AI 처리
	(this->*m_fpMonsterAI[(int)m_ePhase])();
}

// 리틀 데몬 패턴
void CHellBoss::AI_LittleDemonPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		int iRand = rand() % 100;
		// 플레이어가 사정거리 안에 있는가?
		if (PlayerAwareness()) {
			// 70 %
			// 눈깔 빔
			if (0 <= iRand && iRand < 70) {
				goto RETURN_EYEBLAST;
			}
			// 30 %
			// 원거리 공격
			else {
				goto RETURN_SHOOT;
			}
		}
		else {
			// 30 %
			// 눈깔 빔
			if (0 <= iRand && iRand < 30) {
				goto RETURN_EYEBLAST;
			}
			// 70 %
			// 원거리 공격
			else {
				goto RETURN_SHOOT;
			}
		}
	}
	// 플레이어가 멀리 있나
	if (!PlayerBeNear()) {
		goto RETURN_MOVE;
	}
	else {
		goto RETURN_IDLE;
	}
	
RETURN_IDLE:	// 대기
	m_fpAction = &CHellBoss::Action_Idle;
	m_fCountdown = 0.5f;	// 0.5초 대기
	m_wstrTextureKey = L"Com_Texture_LittleDemon_Idle";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 1;
	m_fFrameSpeed = 10.f;
	return;

RETURN_EYEBLAST:	// 눈깔 빔
	m_fpAction = &CHellBoss::Action_LittleDemon_EyeBlast;
	m_wstrTextureKey = L"Com_Texture_LittleDemon_EyeBlast";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 21;
	m_fFrameSpeed = 10.f;
	return;

RETURN_SHOOT:	// 원거리 공격
	m_fpAction = &CHellBoss::Action_LittleDemon_Shoot;
	m_wstrTextureKey = L"Com_Texture_LittleDemon_Shoot";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 26;
	m_fFrameSpeed = 10.f;
	// 목표 = 플레이어 위치 - 몬스터 위치
	m_vAim = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	D3DXVec3Normalize(&m_vAim, &m_vAim);
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellBoss::Action_Move;
	m_fCountdown = ((rand() % 200 + 1) * 0.01) + 1.f;	// 1 ~ 3 초간 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_LittleDemon_Walk")) {
		m_wstrTextureKey = L"Com_Texture_LittleDemon_Walk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 8;
		m_fFrameSpeed = 7.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

// 터보 사탄 패턴
void CHellBoss::AI_TurboSatanPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		// 원거리 공격
		goto RETURN_SHOOT;
	}
	// 플레이어가 멀리 있나
	if (!PlayerBeNear()) {
		goto RETURN_MOVE;
	}

RETURN_SHOOT:	// 원거리 공격
	m_fpAction = &CHellBoss::Action_TurboSatan_ShootStart;
	m_wstrTextureKey = L"Com_Texture_TurboSatan_AttackStart";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 5;
	m_fFrameSpeed = 10.f;
	// 목표 = 플레이어 위치 - 몬스터 위치
	m_vAim = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	D3DXVec3Normalize(&m_vAim, &m_vAim);
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellBoss::Action_Move;
	m_fCountdown = ((rand() % 200 + 1) * 0.01) + 1.f;	// 1 ~ 3 초간 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_TurboSatan_Walk")) {
		m_wstrTextureKey = L"Com_Texture_TurboSatan_Walk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 8;
		m_fFrameSpeed = 9.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

// 부상당한 터보 사탄 패턴
void CHellBoss::AI_InjuredTurboSatanPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		// 원거리 공격
		goto RETURN_SHOOT;
	}
	// 플레이어가 멀리 있나
	if (!PlayerBeNear()) {
		goto RETURN_MOVE;
	}

RETURN_SHOOT:	// 원거리 공격
	m_fpAction = &CHellBoss::Action_InjuredTurboSatan_Shoot;
	m_wstrTextureKey = L"Com_Texture_TurboSatan_DamagedAttack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 16;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellBoss::Action_Move;
	m_fCountdown = ((rand() % 200 + 1) * 0.01) + 1.f;	// 1 ~ 3 초간 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_TurboSatan_DamagedWalk")) {
		m_wstrTextureKey = L"Com_Texture_TurboSatan_DamagedWalk";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 7;
		m_fFrameSpeed = 9.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

// 카코 데빌 패턴
void CHellBoss::AI_CacoDevilPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		int iRand = rand() % 100;
		// 플레이어가 사정거리 안에 있는가?
		if (PlayerAwareness()) {
			// 70 %
			// 근접 충격파
			if (0 <= iRand && iRand < 70) {
				goto RETURN_NOVA;
			}
			// 20 %
			// 눈깔 레이저
			else if (70 <= iRand && iRand < 90) {
				goto RETURN_EYELASERS;
			}
			// 10 %
			// 몬스터 소환
			else {
				goto RETURN_MONSTERSPAWN;
			}
		}
		else {
			// 30 %
			// 눈깔 레이저
			if (0 <= iRand && iRand < 30) {
				goto RETURN_EYELASERS;
			}
			// 70 %
			// 몬스터 소환
			else {
				goto RETURN_MONSTERSPAWN;
			}
		}
	}
	// 플레이어가 멀리 있나
	if (!PlayerBeNear()) {
		goto RETURN_MOVE;
	}

	// 대기
	m_fpAction = &CHellBoss::Action_Idle;
	m_fCountdown = 0.5f;	// 0.5초 대기
	if (m_wstrTextureKey.compare(L"Com_Texture_CacoDevil_Float")) {
		m_wstrTextureKey = L"Com_Texture_CacoDevil_Float";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 9;
		m_fFrameSpeed = 10.f;
	}
	return;

RETURN_EYELASERS:	// 눈깔 레이저
	m_fpAction = &CHellBoss::Action_CacoDevil_EyeLasers;
	m_wstrTextureKey = L"Com_Texture_CacoDevil_EyeLasers";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;
	m_fFrameSpeed = 10.f;
	return;

RETURN_NOVA:	// 근접 충격파
	m_fpAction = &CHellBoss::Action_CacoDevil_Nova;
	m_wstrTextureKey = L"Com_Texture_CacoDevil_Nova";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MONSTERSPAWN:	// 몬스터 소환
	m_fpAction = &CHellBoss::Action_CacoDevil_MonsterSpawn;
	m_wstrTextureKey = L"Com_Texture_CacoDevil_Attack";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 12;
	m_fFrameSpeed = 10.f;
	return;

RETURN_MOVE:	// 이동
	m_fpAction = &CHellBoss::Action_Move;
	m_fCountdown = ((rand() % 200 + 1) * 0.01) + 1.f;	// 1 ~ 3 초간 이동
	if (m_wstrTextureKey.compare(L"Com_Texture_CacoDevil_Float")) {
		m_wstrTextureKey = L"Com_Texture_CacoDevil_Float";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 9;
		m_fFrameSpeed = 10.f;
	}

	// 길찾기
	CMonster::PathFinding(m_pTransformCom->m_TransformDesc.vPosition, m_pPlayer->GetTransform()->m_TransformDesc.vPosition);

	return;
}

// 몰락한 군주 패턴
void CHellBoss::AI_FallenLordPattern()
{
	// 다음 공격 대기 시간까지 기다렸는가
	if (m_fNextAtkWait <= 0) {
		int iRand = rand() % 100;

		// 50 %
		// 촉수 공격
		if (0 <= iRand && iRand < 50) {
			goto RETURN_TENTACLE_ATTACK;
		}
		// 50 %
		// 몬스터 소환
		else {
			goto RETURN_MONSTERSPAWN;
		}
	}

	// 대기
	m_fpAction = &CHellBoss::Action_Idle;
	m_fCountdown = 0.5f;	// 0.5초 대기
	return;

RETURN_TENTACLE_ATTACK:	// 촉수 공격
	m_fpAction = &CHellBoss::Action_FallenLord_TentacleAttack;
	return;

RETURN_MONSTERSPAWN:	// 몬스터 소환
	m_fpAction = &CHellBoss::Action_FallenLord_MonsterSpawn;
	return;
}

// 행동 대기
bool CHellBoss::Action_Idle(float fDeltaTime)
{
	// 지정된 시간만큼 행동 대기
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}
	return false;
}

// 이동
bool CHellBoss::Action_Move(float fDeltaTime)
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

// 페이즈 전환 변신
bool CHellBoss::Action_Morph(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::HPLock); // HP 락 OFF
		//_CollisionComp->bCollision = true;		// 충돌 처리 ON
		return true;
	}

	return false;
}

// 최종 페이즈 변신
bool CHellBoss::Action_LastMorph(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::HPLock); // HP 락 OFF
		//_CollisionComp->bCollision = true;		// 충돌 처리 ON

		m_wstrTextureKey = L"Com_Texture_FallenLord_Idle";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 21;
		m_fFrameSpeed = 10.f;
		return true;
	}

	return false;
}

// 리틀 데몬 눈깔 빔
bool CHellBoss::Action_LittleDemon_EyeBlast(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 1.f;
		return true;
	}

	return false;
}

// 리틀 데몬 원거리 공격
bool CHellBoss::Action_LittleDemon_Shoot(float fDeltaTime)
{
	if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Shoot)) && m_fFrameCnt >= 6.f) {
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Shoot);
		m_iRepeatCount = 6;
		m_fNextAtkWait = 2.f;
	}

	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Shoot) && m_fFrameCnt < 17.f) {
		m_fNextAtkWait -= m_fFrameSpeed * fDeltaTime;
		// 지정된 프레임 마다
		if (m_fNextAtkWait <= 0 && m_iRepeatCount != 0) {
			m_fNextAtkWait = 2.f;
			--m_iRepeatCount;

			// 총알 생성
			BulletBasicArgument* pArg = new BulletBasicArgument;
			pArg->uiSize = sizeof(BulletBasicArgument);
			pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition;	// 생성 위치
			pArg->vDir = m_vAim;	// 방향
			m_pManagement->AddScheduledGameObjectInLayer(
				(_int)ESceneID::Static,
				L"GameObject_HellBossRingBullet",
				L"Layer_Bullet",
				nullptr, (void*)pArg);
		}
	}

	if (m_bFrameLoopCheck) {
		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::Shoot);
		m_fNextAtkWait = 1.f;
		return true;
	}

	return false;
}

// 터보 사탄 원거리 공격 시작
bool CHellBoss::Action_TurboSatan_ShootStart(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fpAction = &CHellBoss::Action_TurboSatan_ShootSpin;
		m_wstrTextureKey = L"Com_Texture_TurboSatan_AttackSpin";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
	}

	return false;
}

// 터보 사탄 원거리 공격 회전
bool CHellBoss::Action_TurboSatan_ShootSpin(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fpAction = &CHellBoss::Action_TurboSatan_ShootFire;
		m_wstrTextureKey = L"Com_Texture_TurboSatan_AttackFire";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		m_fFrameSpeed = 20.f;

		m_iRepeatCount = (rand() % 8 + 1) + 2;	// 2 ~ 10번 반복
	}

	return false;
}

// 터보 사탄 원거리 공격 발싸
bool CHellBoss::Action_TurboSatan_ShootFire(float fDeltaTime)
{
	if (!(m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Shoot)) && m_fFrameCnt >= 0.f) {
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Shoot);
		m_iRepeatCount = 2;
		m_fNextAtkWait = 2.f;
	}

	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Shoot) && m_fFrameCnt < 3.f) {
		m_fNextAtkWait -= m_fFrameSpeed * fDeltaTime;
		// 지정된 프레임 마다
		if (m_fNextAtkWait <= 0 && m_iRepeatCount != 0) {
			m_fNextAtkWait = 2.f;
			--m_iRepeatCount;

			// 총알 생성
			BulletBasicArgument* pArg = new BulletBasicArgument;
			pArg->uiSize = sizeof(BulletBasicArgument);
			pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition;	// 생성 위치
			pArg->vDir = m_vAim;	// 방향
			m_pManagement->AddScheduledGameObjectInLayer(
				(_int)ESceneID::Static,
				L"GameObject_HellBossRingBullet",
				L"Layer_Bullet",
				nullptr, (void*)pArg);
		}
	}

	if (m_bFrameLoopCheck) {
		if (m_iRepeatCount != 0) {
			--m_iRepeatCount;
			return false;
		}

		m_byMonsterFlag &= ~static_cast<BYTE>(MonsterFlag::Shoot);

		m_fpAction = &CHellBoss::Action_TurboSatan_ShootEnd;
		m_wstrTextureKey = L"Com_Texture_TurboSatan_AttackEnd";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		m_fFrameSpeed = 10.f;
	}

	return false;
}

// 터보 사탄 원거리 공격 끝
bool CHellBoss::Action_TurboSatan_ShootEnd(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 3.f;
		return true;
	}

	return false;
}

// 부상당한 리틀 데몬 원거리 공격
bool CHellBoss::Action_InjuredTurboSatan_Shoot(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 3.f;
		return true;
	}

	return false;
}

// 카코 데빌 눈깔 레이저
bool CHellBoss::Action_CacoDevil_EyeLasers(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 3.f;
		return true;
	}

	return false;
}

// 카코 데빌 근접 충격파
bool CHellBoss::Action_CacoDevil_Nova(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 5.f;
		return true;
	}

	return false;
}

// 카코 데빌 몬스터 소환
bool CHellBoss::Action_CacoDevil_MonsterSpawn(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 4.f;
		return true;
	}

	return false;
}

bool CHellBoss::Action_FallenLord_TentacleAttack(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 2.f;
		return true;
	}

	return false;
}

bool CHellBoss::Action_FallenLord_MonsterSpawn(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_fNextAtkWait = 2.f;
		return true;
	}

	return false;
}

// 죽음
bool CHellBoss::Action_Dead(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::Dead);	// 몬스터가 죽었어요
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		return false;
	}

	return false;
}

CHellBoss* CHellBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBoss* pInstance = new CHellBoss(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBoss");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBoss::Clone(void* pArg/* = nullptr*/)
{
	CHellBoss* pClone = new CHellBoss(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBoss");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBoss::Free()
{
	CMonster::Free();
}
