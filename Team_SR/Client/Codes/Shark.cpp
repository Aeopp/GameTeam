#include "stdafx.h"
#include "..\Headers\Shark.h"
#include "SharkBullet.h"
#include "Player.h"

CShark::CShark(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice),m_wstrBase(L""),m_fpSharkAI{},m_fpAction(nullptr)
{
}

HRESULT CShark::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;
	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CShark>();
	return S_OK;
}

HRESULT CShark::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = m_wstrBase + L"Walk_1Phase";

	m_pTransformCom->m_TransformDesc.vScale = { 8.f, 8.f, 8.f };
	bGravity = false;
	m_stOriginStatus.fHP = 1200.f;
	m_stOriginStatus.fATK = 15.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 15.f;
	m_stOriginStatus.fMeleeRange = 5.f;
	m_stOriginStatus.fDetectionRange = 50.f;

	m_stStatus = m_stOriginStatus;

	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 6.f;
	m_fFrameSpeed = 1.f;
	m_iDir = 1.f;
	m_fpAction = &CShark::Action_Idle;
	m_fpSharkAI[(int)PHASE::HP_High] = &CShark::AI_FirstPhase;
	m_fpSharkAI[(int)PHASE::HP_Middle] = &CShark::AI_SecondPhase;
	m_fpSharkAI[(int)PHASE::HP_Low] = &CShark::AI_ThridPhase;
	m_fpSharkAI[(int)PHASE::HP_ZERO] = &CShark::AI_DeadPhase;

	return S_OK;
}

_uint CShark::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);


	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}

	Update_AI(fDeltaTime);
	if (&CShark::Action_Shoot == m_fpAction)
		CreateBullet(fDeltaTime);
	UpdateDir(fDeltaTime);
	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CShark::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);

	return _uint();
}

HRESULT CShark::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	_CollisionComp->DebugDraw();

	return S_OK;
}

void CShark::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소
	//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SHARK);
	//CSoundMgr::Get_Instance()->PlaySound(L"Bat_pain_01.wav", CSoundMgr::SHARK);
	CMonster::CreateBlood();
}

void CShark::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
}

void CShark::Update_AI(float fDeltaTime)
{
	m_fNextAtkWait -= fDeltaTime;

	if (!(this->*m_fpAction)(fDeltaTime)) 
	{
		return;
	}

	if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.66f) 
	{
		m_ePhase = PHASE::HP_High;	
	}
	else if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.33f &&
		m_stStatus.fHP <= m_stOriginStatus.fHP * 0.66f)
	{
		m_ePhase = PHASE::HP_Middle;	
	}
	else if (m_stStatus.fHP <= m_stOriginStatus.fHP * 0.33f &&
		m_stStatus.fHP > 0.f)
	{
		m_ePhase = PHASE::HP_Low;
	}
	else if (m_stStatus.fHP <= 0)
	{
		m_ePhase = PHASE::HP_ZERO;
	}
	(this->*m_fpSharkAI[(int)m_ePhase])();
}

void CShark::AI_FirstPhase()
{
	int iRand = rand() % 100;

	if (iRand < 50)
	{
		m_fpAction = &CShark::Action_Move;
		m_fCountDown = 1.5f;
		m_wstrTextureKey = m_wstrBase + L"Walk_1Phase";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 7;
		m_fFrameSpeed = 5.f;
	}
	else
	{
		m_fpAction = &CShark::Action_Shoot;
		m_wstrTextureKey = m_wstrBase + L"RangeAttack_1Phase";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 11;
		m_fFrameSpeed = 10.f;
	}
}

void CShark::AI_SecondPhase()
{
	int iRand = rand() % 100;

	if (iRand < 50)
	{
		m_fpAction = &CShark::Action_Move;
		m_fCountDown = 1.5f;
		m_wstrTextureKey = m_wstrBase + L"Walk_2Phase";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 7;
		m_fFrameSpeed = 5.f;
	}
	else
	{
		m_fpAction = &CShark::Action_Shoot;
		m_wstrTextureKey = m_wstrBase + L"RangeAttack_2Phase";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 11;
		m_fFrameSpeed = 10.f;
	}

}

void CShark::AI_ThridPhase()
{
	m_stStatus.fSpeed = 25.f;
	m_fpAction = &CShark::Action_Move;
	m_fCountDown = 1.5f;
	m_wstrTextureKey = m_wstrBase + L"Walk_3Phase";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 9;
	m_fFrameSpeed = 5.f;

}

void CShark::AI_DeadPhase()
{
	m_fpAction = &CShark::Action_Death;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Death";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 13.f;
}

bool CShark::Action_Move(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	const _vector vEyebatPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vLook = vPlayerPos - vEyebatPos;
	vLook.y = 0.f;
	float fLookLength = D3DXVec3Length(&vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (fLookLength > 10)
	{
		m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * m_stStatus.fSpeed;
		if (PHASE::HP_Middle == m_ePhase)
			m_pTransformCom->m_TransformDesc.vPosition.x += m_iDir;
	}
	if (fLookLength <= 10)
	{
		if (PHASE::HP_High == m_ePhase)
		{
			m_wstrTextureKey = m_wstrBase + L"Melee_1Phase";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 14;
			m_fFrameSpeed = 10.f;	
		}
		else if (PHASE::HP_Middle == m_ePhase)
		{
			m_wstrTextureKey = m_wstrBase + L"Melee";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 11;
			m_fFrameSpeed = 10.f;
		}
		else if (PHASE::HP_Low == m_ePhase)
		{
			m_wstrTextureKey = m_wstrBase + L"Howling";
			m_fFrameCnt = 0;
			m_fStartFrame = 0;
			m_fEndFrame = 10;
			m_fFrameSpeed = 10.f;
		}
		m_fpAction = &CShark::Action_MeleeAttack;

	}



	return false;
}

bool CShark::Action_Idle(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}

	return false;
}

bool CShark::Action_Shoot(float fDeltaTime)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SHARK);
	CSoundMgr::Get_Instance()->PlaySound(L"shambler_attack_3.wav", CSoundMgr::SHARK);
	if (m_bFrameLoopCheck)
	{
		//CreateBullet(fDeltaTime);
		m_iBulletCount = 0;
		m_fNextAtkWait = 2.f;
		return true;
	}

	return false;
}

bool CShark::Action_Dash(float fDeltaTime)
{
	if (m_bFrameLoopCheck)
	{
		m_fNextAtkWait = 3.f;
		return true;
	}
	return false;
}

bool CShark::Action_Warigari(float fDeltaTime)
{

	return false;
}

bool CShark::Action_HighJump(float fDeltaTime)
{
	return false;
}

bool CShark::Action_MeleeAttack(float fDeltaTime)
{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SHARK);
		CSoundMgr::Get_Instance()->PlaySound(L"shambler_attack_2.wav", CSoundMgr::SHARK);
	if (m_bFrameLoopCheck) 
	{
		m_fNextAtkWait = 1.f;
		_vector AttackDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		D3DXVec3Normalize(&AttackDir, &AttackDir);
		Ray _Ray;
		_Ray.Direction = AttackDir;
		_Ray.Start = m_pTransformCom->m_TransformDesc.vPosition;
		CMonster::Attack(_Ray, 10.f);
		return true;
	}

	return false;
}

bool CShark::Action_Death(float fDeltaTime)
{
	if (m_fFrameCnt >= m_fEndFrame - 1)
	{
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		m_byObjFlag |= (BYTE)ObjFlag::Remove;
		m_byMonsterFlag |= (BYTE)MonsterFlag::Dead;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SHARK);
		CSoundMgr::Get_Instance()->PlaySound(L"shambler_death_bloody.wav", CSoundMgr::SHARK);
		
	}

	return false;
}

void CShark::CreateBullet(float fDeltaTime)
{
	_vector vPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	//for (int i = 0; i < 6; ++i)
	//{
	//	vPos += vDir * (i + 1);
	//	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
	//		CGameObject::Tag + TYPE_NAME<CSharkBullet>(),
	//		(_int)ESceneID::Stage1st,
	//		CGameObject::Tag + TYPE_NAME<CMonster>(),
	//		nullptr, (void*)&vPos)))
	//		return;
	//}

	m_fBulletCoolDown += fDeltaTime;
	if (m_fBulletCoolDown > 0.2f && m_iBulletCount < 6)
	{
		m_fBulletCoolDown = 0.f;
		vPos += vDir * ((m_iBulletCount + 1)*6);
		if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CSharkBullet>(),
			(_int)ESceneID::Stage1st,
			CGameObject::Tag + TYPE_NAME<CMonster>(),
			nullptr, (void*)&vPos)))
			return;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GALICER);
		CSoundMgr::Get_Instance()->PlaySound(L"ice_breaking_1.wav", CSoundMgr::GALICER);
		++m_iBulletCount;
	}

	//while (m_iBulletCount < 6)
	//{
	//	m_fBulletCoolDown += fDeltaTime;
	//	if (m_fBulletCoolDown > 1.f)
	//	{
	//		vPos += vDir * (m_iBulletCount + 1);
	//		if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
	//			CGameObject::Tag + TYPE_NAME<CSharkBullet>(),
	//			(_int)ESceneID::Stage1st,
	//			CGameObject::Tag + TYPE_NAME<CMonster>(),
	//			nullptr, (void*)&vPos)))
	//			return;
	//		m_fBulletCoolDown = 0.f;
	//		++m_iBulletCount;
	//	}
	//}
	//m_iBulletCount = 0;
}

void CShark::UpdateDir(float fDeltaTime)
{
	m_fChagneDirTime += fDeltaTime;
	if (m_fChagneDirTime >= 0.3f)
	{
		m_fChagneDirTime = 0.f;
		m_iDir *= -1;
	}
}

HRESULT CShark::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	CTexture* pTexture = nullptr;

#pragma region Texture

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Death",
		L"Com_Texture_SharkDeath",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Death", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Howling",
		L"Com_Texture_SharkHowling",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Howling", pTexture);

	//if (FAILED(CGameObject::AddComponent(
	//	(_int)ESceneID::Static,
	//	m_wstrBase + L"icicleA",
	//	L"Com_Texture_SharkicicleA",
	//	(CComponent**)&pTexture)))
	//	return E_FAIL;
	//m_mapTexture.emplace(m_wstrBase + L"icicleA", pTexture);

	//if (FAILED(CGameObject::AddComponent(
	//	(_int)ESceneID::Static,
	//	m_wstrBase + L"icicleB",
	//	L"Com_Texture_icicleB",
	//	(CComponent**)&pTexture)))
	//	return E_FAIL;
	//m_mapTexture.emplace(m_wstrBase + L"icicleB", pTexture);

	//if (FAILED(CGameObject::AddComponent(
	//	(_int)ESceneID::Static,
	//	m_wstrBase + L"icicleC",
	//	L"Com_Texture_SharkicicleC",
	//	(CComponent**)&pTexture)))
	//	return E_FAIL;
	//m_mapTexture.emplace(m_wstrBase + L"icicleC", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Death",
		L"Com_Texture_LoseBothArm",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"LoseBothArm", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"LoseRightArm",
		L"Com_Texture_LoseRightArm",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"LoseRightArm", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Melee",
		L"Com_Texture_SharkMelee",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Melee", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Melee_1Phase",
		L"Com_Texture_SharkMelee_1Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Melee_1Phase", pTexture);

	

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"RangeAttack_1Phase",
		L"Com_Texture_SharkRangeAttack_1Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"RangeAttack_1Phase", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"RangeAttack_2Phase",
		L"Com_Texture_SharkRangeAttack_2Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"RangeAttack_2Phase", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Walk_1Phase",
		L"Com_Texture_SharkWalk_1Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Walk_1Phase", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Walk_2Phase",
		L"Com_Texture_SharkWalk_2Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Walk_2Phase", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Walk_3Phase",
		L"Com_Texture_SharkWalk_3Phase",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Walk_3Phase", pTexture);

#pragma endregion



	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 4.f;
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

HRESULT CShark::Set_Texture()
{

	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

CShark * CShark::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CShark* pInstance = new CShark(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CShark");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CShark::Clone(void * pArg /*= nullptr*/)
{
	CShark* pClone = new CShark(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CShark");
		SafeRelease(pClone);
	}

	return pClone;
}

void CShark::Free()
{
	CMonster::Free();
}
