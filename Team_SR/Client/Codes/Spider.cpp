#include "stdafx.h"
#include "..\Headers\Spider.h"
#include "Camera.h"


CSpider::CSpider(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}

HRESULT CSpider::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;
	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CSpider>();

	GibTable = { 1,2,5,11,9,28,29,41,40,34,43 };

	return S_OK;
}

HRESULT CSpider::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = m_wstrBase + L"Walk";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 3;
	m_fFrameSpeed = 20.f;
	//m_pTransformCom->m_TransformDesc.vRotation.z = -90.f;
	
	bGravity = false;
	m_stOriginStatus.fHP = 60.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 8.f;
	m_stOriginStatus.fDetectionRange = 30.f;
	m_stOriginStatus.fMeleeRange = 15.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	m_fJumpPower = 12.f;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	m_fpAction = &CSpider::Action_Idle;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::WALL] = &CSpider::AI_WallPhase;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::FLOOR] = &CSpider::AI_NoAwareness;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::DEATH] = &CSpider::AI_DeadPhase;

	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::WALL] = &CSpider::AI_WallPhase;		// 적극적으로 공격
	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::FLOOR] = &CSpider::AI_FloorPhase;
	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::DEATH] = &CSpider::AI_DeadPhase;
	
	m_ePhase = PHASE::FLOOR;
	m_eAwareness = AWARENESS::No;
	return S_OK;
}

_uint CSpider::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}
	if (LightHitTime > 0.0f)return 0;

	//if (GetAsyncKeyState('6') & 0x8000)
	//{
	//	m_eAwareness = AWARENESS::Yes;
	//	m_bTest = true;
	//	m_vLook = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	//	m_vLook.y = 0.f;
	//	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	//}

	if (false == m_bTest
		&& m_pTransformCom->m_TransformDesc.vPosition.y > 13)
		m_vRandomLook.y *= -1.f; /** m_stStatus.fSpeed*/;
	if (true == m_bTest)
		Jump(fDeltaTime);
	Update_AI(fDeltaTime);
	ChagneSpeed(fDeltaTime);
	
	_CollisionComp->Update(m_pTransformCom);
	

	return _uint();
}

_uint CSpider::LateUpdateGameObject(float fDeltaTime)
{
	if(PHASE::FLOOR ==  m_ePhase || m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead))
		CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);

	return _uint();
}

HRESULT CSpider::RenderGameObject()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;


	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	_CollisionComp->DebugDraw();

	return S_OK;
}

void CSpider::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return;
	}
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	if (&CSpider::Action_Move_NoAwareness == m_fpAction)
	{
		m_eAwareness = AWARENESS::Yes;
		m_ePhase = PHASE::FLOOR;
	}


	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
	CSoundMgr::Get_Instance()->PlaySound(L"Bat_pain_03.wav", CSoundMgr::SPIDER);
												// 충돌 관련 정보
	//m_vCollisionDir = _CollisionInfo.Dir;
	//m_fCrossValue = _CollisionInfo.CrossValue;

	//CMonster::CreateBlood();
}

void CSpider::ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)
{	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return;
	}
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	if (&CSpider::Action_Move_NoAwareness == m_fpAction)
	{
		m_eAwareness = AWARENESS::Yes;
		m_ePhase = PHASE::FLOOR;
	}


	CMonster::ParticleHit(_Particle, _CollisionInfo);		// CMonster 에서 HP 감소

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
	CSoundMgr::Get_Instance()->PlaySound(L"Bat_pain_03.wav", CSoundMgr::SPIDER);
	// 충돌 관련 정보
//m_vCollisionDir = _CollisionInfo.Dir;
//m_fCrossValue = _CollisionInfo.CrossValue;

//CMonster::CreateBlood();
}

void CSpider::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	//if (PHASE::WALL == m_ePhase && false == m_bTest)
	//	return;
	if (_CollisionInfo.Flag == L"Wall" && false == m_bTest
		&& AWARENESS::No == m_eAwareness)
	{
		m_ePhase = PHASE::WALL;
		m_eAwareness = AWARENESS::Yes;
		m_fpAction = &CSpider::Action_Idle;
		if (m_vRandomLook.x < 0)
			m_pTransformCom->m_TransformDesc.vRotation.z = -90.f;
		else if (m_vRandomLook.x > 0)
			m_pTransformCom->m_TransformDesc.vRotation.z = 90.f;
	}
	else if (_CollisionInfo.Flag == L"Floor")
	{
		//m_ePhase = PHASE::FLOOR;
		
		if (m_bTest)
		{
			m_bTest = false;
			m_ePhase = PHASE::FLOOR;
			m_pTransformCom->m_TransformDesc.vRotation.z = 0.f;
			m_pTransformCom->m_TransformDesc.vPosition.y = _CollisionInfo.IntersectPoint.y;
		}
	}
	//바닥이랑 충돌하면 각도를 정면 보게

	//else
	//	m_pTransformCom->m_TransformDesc.vPosition.y += 0.5f;
}

void CSpider::Update_AI(float fDeltaTime)
{
	if ((this->*m_fpAction)(fDeltaTime))
	{
		// 플레이어를 인식했는가?
		//if (PlayerAwareness()) {
		//	m_eAwareness = AWARENESS::Yes;
		//	if (PHASE::WALL == m_ePhase)
		//	{
		//		//int i = 0;
		//		//if (true == m_bTest)
		//		//	return;
		//		//m_eAwareness = AWARENESS::Yes;
		//		//m_bTest = true;
		//		//m_vLook = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		//		//m_vLook.y = 0.f;
		//		//m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
		//	}
		//}
		//else {
		//	m_eAwareness = AWARENESS::No;
		//}

		if (m_stStatus.fHP <= 0)
		{
			m_ePhase = PHASE::DEATH;
			m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::HPLock);
		}



		(this->*m_fpSpiderAI[(int)m_eAwareness][(int)m_ePhase])();
	}

}

void CSpider::Z_Billboard()
{
	//CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, L"Layer_MainCamera");
	//if (nullptr == pCamera)
	//	return E_FAIL;
	//const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;

	//vec3 vecPos = m_pTransformCom->m_TransformDesc.vPosition;
	//vec3 vecRot = m_pTransformCom->m_TransformDesc.vRotation;
	//vec3 vecScale = m_pTransformCom->m_TransformDesc.vScale;

	//D3DXMATRIX matScale;
	//D3DXMATRIX matPosition;
	//D3DXMATRIX matRX, matRY, matRZ;
	//D3DXMATRIX matbillboard, matView, matWorld;
	//D3DXMatrixScaling(&matScale, vecScale.x, vecScale.y, vecScale.z);
	//D3DXMatrixRotationX(&matRX, D3DXToRadian(vecRot.x));
	//D3DXMatrixRotationY(&matRY, D3DXToRadian(vecRot.y));
	//D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vecRot.z));
	//D3DXMatrixTranslation(&matPosition, vecPos.x, vecPos.y, vecPos.z);

	//matView = pCamera->GetCameraDesc().matView;
	//D3DXMatrixIdentity(&matbillboard);

	//matbillboard._11 = matView._11;
	//matbillboard._13 = matView._13;
	//matbillboard._31 = matView._31;
	//matbillboard._33 = matView._33;

	//D3DXMatrixInverse(&matbillboard, 0, &matbillboard);

	//matWorld = matScale *  matRX * matRY * matRZ * matbillboard * matPosition;

	//m_pTransformCom->m_TransformDesc.matWorld = matWorld;
}

void CSpider::ChagneSpeed(float fDeltaTime)
{
	m_fChangeSpeed += fDeltaTime;
	if (m_fChangeSpeed >= 0.5f)
	{
		m_fChangeSpeed = 0.f;
		float fRandomX = rand() % 3 - 1;
		if (PHASE::WALL == m_ePhase)
			m_vRandomLook = { 0.f,1.f,0.f };
		//else if (PHASE::FLOOR == m_ePhase)
		//{
		//	m_vRandomLook = { fRandomY,0.f,fRandomZ };
		//}
		else if (PHASE::FLOOR == m_ePhase)
		{
			//float fRandom = rand() % 100;
			//if(fRandom < 50)
			//	m_vRandomLook = { 0.f,0.f,fRandomZ };
			//else
			m_vRandomLook = { fRandomX,0.f,0.f };
		}
	}
}

void CSpider::Jump(float fDeltaTime)
{
	m_fTest += fDeltaTime * 1.f;
	float Test = MATH::Lerp(m_pTransformCom->m_TransformDesc.vRotation.z, 0.f, m_fTest);
	m_pTransformCom->m_TransformDesc.vRotation.z = Test;
	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime/* * 2.f*/;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);

	if (m_fJumpTime < 3.f)
		m_fJumpTime += 0.03f;
}

void CSpider::AI_NoAwareness()
{
	m_fpAction = &CSpider::Action_Move_NoAwareness;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Walk";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fFrameSpeed = 20.f;
	m_fEndFrame = 3.f;
	m_stStatus.fSpeed = 10.f;
}

void CSpider::AI_WallPhase()
{
	if (m_bTest)
		return;
	if (m_stStatus.fHP < m_stOriginStatus.fHP)
	{
		m_vRandomLook.y = 1.f;
		m_eAwareness = AWARENESS::Yes;
		m_bTest = true;
		m_vLook = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		m_vLook.y = 0.f;
		m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	}

	int iRand = rand() % 100;

	// 30 %
	// 대기
	if (iRand < 70) 
	{
		m_fpAction = &CSpider::Action_Move_Wall;
		m_fCountDown = 1.f;
		m_wstrTextureKey = m_wstrBase + L"Walk";
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fFrameSpeed = 20.f;
		m_fEndFrame = 3.f;
	}
	else
	{
		m_fpAction = &CSpider::Action_Shoot;
		m_fCountDown = 1.f;
		m_wstrTextureKey = m_wstrBase + L"Attack";
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fFrameSpeed = 20.f;
		m_fEndFrame = 4.f;
	}

	




}

void CSpider::AI_FloorPhase()
{
	//이동
	m_fpAction = &CSpider::Action_Move_Floor;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Walk";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 3.f;
	m_fFrameSpeed = 20.f;
	m_stStatus.fSpeed = 25.f;
	//근접공격


}

void CSpider::AI_DeadPhase()
{
	m_fpAction = &CSpider::Action_Death;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Death";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fFrameSpeed = 5.f;
	m_fEndFrame = 12.f;
}

bool CSpider::Action_Move_NoAwareness(float fDeltaTime)
{
	if (PHASE::FLOOR == m_ePhase)
		m_pTransformCom->m_TransformDesc.vPosition += m_vRandomLook * fDeltaTime * m_stStatus.fSpeed;

	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CSpider::Action_Move_Wall(float fDeltaTime)
{

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
	CSoundMgr::Get_Instance()->PlaySound(L"Spider_detect_03.wav", CSoundMgr::SPIDER);

	m_pTransformCom->m_TransformDesc.vPosition += m_vRandomLook * fDeltaTime * m_stStatus.fSpeed;

	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CSpider::Action_Move_Floor(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	vDir.y = 0.f;
	float fLookLength = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	// 포지션 이동
	if (fLookLength > 5)
		m_pTransformCom->m_TransformDesc.vPosition += vDir * m_stStatus.fSpeed * fDeltaTime;
	else
	{
		m_fCountDown = 1.f;
		m_wstrTextureKey = m_wstrBase + L"Attack";
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fFrameSpeed = 10.f;
		m_fEndFrame = 13.f;
		m_fpAction = &CSpider::Action_Mellee_Attack;
	}

	if (m_fCountDown <= 0)
	{
		return true;
	}


	return false;
}

bool CSpider::Action_Idle(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CSpider::Action_Shoot(float fDeltaTime)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
	CSoundMgr::Get_Instance()->PlaySound(L"Spider_attack_02.wav", CSoundMgr::SPIDER);
	if (m_bFrameLoopCheck)
	{
		CreateBullet();
		return true;
	}
	return false;
}

bool CSpider::Action_Death(float fDeltaTime)
{
	if (m_fFrameCnt >= m_fEndFrame - 1)
	{
		m_fFrameCnt = m_fEndFrame - 1;
		m_fStartFrame = m_fEndFrame - 1;
		//m_byObjFlag |= (BYTE)ObjFlag::Remove;
		m_byMonsterFlag |= (BYTE)MonsterFlag::Dead;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
		CSoundMgr::Get_Instance()->PlaySound(L"Spider_death_01.wav", CSoundMgr::SPIDER);
	}

	return false;
}

bool CSpider::Action_Mellee_Attack(float fDeltaTime)
{
	m_fCountDown -= fDeltaTime;
	//_vector AttackDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	//D3DXVec3Normalize(&AttackDir, &AttackDir);
	//Sphere _Sphere;
	//_Sphere.Center = m_pTransformCom->m_TransformDesc.vPosition + (AttackDir);
	//_Sphere.Radius = 10;
	//CMonster::Attack(_Sphere, 30.f);
	if (m_fCountDown <= 0)
	{
		//_vector AttackDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		//D3DXVec3Normalize(&AttackDir, &AttackDir);
		//Ray _Ray;
		//_Ray.Direction = AttackDir;
		//_Ray.Start = m_pTransformCom->m_TransformDesc.vPosition;
		//CMonster::Attack(_Ray, 10.f);
		CMonster::MeleeAttack();
		return true;
	}
	return false;
}

void CSpider::CreateBullet()
{
	_vector pPositionArr[2];
	pPositionArr[0] = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	pPositionArr[1] = m_pTransformCom->m_TransformDesc.vPosition;
	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag +  L"EyebatBullet",
		(_int)ESceneID::StageMidBoss,
		CGameObject::Tag + L"EyebatBullet",
		nullptr, (void*)pPositionArr)))
		return;
}

HRESULT CSpider::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	wstring wstrTextureSpider = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CSpider>();

	CTexture* pTexture;
	// Walk
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Walk",
		L"Com_Texture_SpiderWalk",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(wstrTextureSpider + L"Walk", pTexture);
	// Death
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Death",
		L"Com_Texture_SpiderDeath",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(wstrTextureSpider + L"Death", pTexture);
	// Attack
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Attack",
		L"Com_Texture_SpiderAttack",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(wstrTextureSpider + L"Attack", pTexture);
	// Hit
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureSpider + L"Hit",
		L"Com_Texture_SpiderHit",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(wstrTextureSpider + L"Hit", pTexture);

	// Collision
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.5f;
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

HRESULT CSpider::Set_Texture()
{
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

CSpider * CSpider::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSpider* pInstance = new CSpider(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CSpider");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CSpider::Clone(void * pArg /*= nullptr*/)
{
	CSpider* pClone = new CSpider(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CSpider");
		SafeRelease(pClone);
	}

	return pClone;
}

void CSpider::Free()
{
	//SafeRelease(_CollisionComp);

	CMonster::Free();
}

void CSpider::FreezeHit()
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return;
	}
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	if (&CSpider::Action_Move_NoAwareness == m_fpAction)
	{
		m_eAwareness = AWARENESS::Yes;
		m_ePhase = PHASE::FLOOR;
	}


	CMonster::FreezeHit();		// CMonster 에서 HP 감소

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SPIDER);
	CSoundMgr::Get_Instance()->PlaySound(L"Bat_pain_03.wav", CSoundMgr::SPIDER);
}
