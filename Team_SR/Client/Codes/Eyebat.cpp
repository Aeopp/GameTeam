#include "stdafx.h"
#include "..\Headers\Eyebat.h"
#include "EyebatBullet.h"


CEyebat::CEyebat(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}

HRESULT CEyebat::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;
	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CEyebat>();

	GibTable = {8,8,7,1,2,6,8,37,39,32,23,23 };


	return S_OK;
}

HRESULT CEyebat::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = m_wstrBase + L"Fly";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 6;

	//테스트
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	m_iDir = 1;
	//
	bGravity = false;

	m_stOriginStatus.fHP = 80.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fDetectionRange = 50.f;

	m_stStatus = m_stOriginStatus;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	m_fpAction = &CEyebat::Action_Idle;
	m_fpEyebatAI[(int)AWARENESS::No][(int)PHASE::HP_High] = &CEyebat::AI_NoAwareness;
	m_fpEyebatAI[(int)AWARENESS::No][(int)PHASE::HP_Low] = &CEyebat::AI_NoAwareness;
	m_fpEyebatAI[(int)AWARENESS::No][(int)PHASE::HP_ZERO] = &CEyebat::AI_NoAwareness;

	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_High] = &CEyebat::AI_FirstPhase;		// 적극적으로 공격
	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_Low] = &CEyebat::AI_SecondPhase;	// 소극적으로 공격
	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_ZERO] = &CEyebat::AI_DeadPhase;	// 소극적으로 공격

	return S_OK;
}

_uint CEyebat::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	//테스트
	//if (GetAsyncKeyState('5') & 0x8000)
	//	m_stStatus.fHP -= 1.f;

	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}

	if (LightHitTime > 0.0f)return 0;

	Update_AI(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);
	return _uint();
}

_uint CEyebat::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);



	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);


	return _uint();
}

HRESULT CEyebat::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	//_CollisionComp->DebugDraw();

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(Set_Texture()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

// 몬스터가 피해를 받음
void CEyebat::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EYEBAT);
	CSoundMgr::Get_Instance()->PlaySound(L"Bat_pain_01.wav", CSoundMgr::EYEBAT);
	// 충돌 관련 정보
	m_vCollisionDir = _CollisionInfo.Dir;
	m_fCrossValue = _CollisionInfo.CrossValue;
	CMonster::CreateBlood();
}

void CEyebat::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
}

void CEyebat::Update_AI(float fDeltaTime)
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

		if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.5f) {
			m_ePhase = PHASE::HP_High;
		}
		else if (m_stStatus.fHP < m_stOriginStatus.fHP * 0.5f
			&& m_stStatus.fHP > 0)
		{
			m_ePhase = PHASE::HP_Low;
		}
		else if (m_stStatus.fHP < 0)
		{
			m_ePhase = PHASE::HP_ZERO;
		}

		(this->*m_fpEyebatAI[(int)m_eAwareness][(int)m_ePhase])();
	}

}

void CEyebat::AI_NoAwareness()
{
	m_fpAction = &CEyebat::Action_Idle;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Fly";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 6.f;
}

void CEyebat::AI_FirstPhase()
{
	m_fpAction = &CEyebat::Action_Move;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Fly";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 6.f;
}

void CEyebat::AI_SecondPhase()
{
	m_fpAction = &CEyebat::Action_Shoot;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Attack";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 6.f;
}

void CEyebat::AI_DeadPhase()
{
	m_fpAction = &CEyebat::Action_Death;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Death";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 11.f;
}

bool CEyebat::Action_Move(float fDeltaTime)
{
	const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	const _vector vEyebatPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vLook = vPlayerPos - vEyebatPos;
	vLook.y = 0.f;
	float fLookLength = D3DXVec3Length(&vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (fLookLength > 15)
		m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * m_stStatus.fSpeed;

	m_pTransformCom->m_TransformDesc.vPosition.y += fDeltaTime * m_iDir * 2;
	if (m_pTransformCom->m_TransformDesc.vPosition.y > m_fStartY + 2)
		m_iDir *= -1;
	else if(m_pTransformCom->m_TransformDesc.vPosition.y < m_fStartY - 2)
		m_iDir *= -1;
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}

	return false;
}

bool CEyebat::Action_Idle(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CEyebat::Action_Shoot(float fDeltaTime)
{
	if (m_bFrameLoopCheck)
	{
		CreateBullet();
		return true;
	}

	return false;
}

bool CEyebat::Action_Death(float fDeltaTime)
{
	if (m_fFrameCnt >= m_fEndFrame - 1)
	{
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		m_byObjFlag |= (BYTE)ObjFlag::Remove;
		m_byMonsterFlag |= (BYTE)MonsterFlag::Dead;
		CMonster::CreateFloorBlood();
	}

	return false;
}

void CEyebat::CreateBullet()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EYEBAT);
	CSoundMgr::Get_Instance()->PlaySound(L"Incubus_attack_01.wav", CSoundMgr::EYEBAT);
	_vector pPositionArr[2];
	pPositionArr[0] = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	pPositionArr[1] = m_pTransformCom->m_TransformDesc.vPosition;
	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CEyebatBullet>(),
		(_int)ESceneID::Stage1st,
		CGameObject::Tag + TYPE_NAME<CEyebatBullet>(),
		nullptr, (void*)pPositionArr)))
		return;
}


HRESULT CEyebat::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	CTexture* pTexture = nullptr;

	//Fly
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Fly",
		L"Com_Texture_EyebatFly",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Fly", pTexture);

	//Attack
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Attack",
		L"Com_Texture_EyebatAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Attack", pTexture);

	//Death
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Death",
		L"Com_Texture_EyebatDeath",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Death", pTexture);

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

HRESULT CEyebat::Set_Texture()
{

	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

CEyebat * CEyebat::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CEyebat* pInstance = new CEyebat(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CEyebat");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEyebat::Clone(void * pArg /*= nullptr*/)
{
	CEyebat* pClone = new CEyebat(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CEyebat");
		SafeRelease(pClone);
	}

	return pClone;
}

void CEyebat::Free()
{	
	// 2020.12.17 11:26 KMJ
	// CMonster 에서
	//SafeRelease(_CollisionComp);

	CMonster::Free();
}
