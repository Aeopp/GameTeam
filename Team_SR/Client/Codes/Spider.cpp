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
	m_pTransformCom->m_TransformDesc.vRotation.z = -90.f;

	m_stOriginStatus.fHP = 100.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 2.f;
	m_stOriginStatus.fDetectionRange = 100.f;
	m_stOriginStatus.fMeleeRange = 15.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	m_fJumpPower = 15.f;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	m_fpAction = &CSpider::Action_Idle;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::WALL] = &CSpider::AI_NoAwareness;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::FLOOR] = &CSpider::AI_NoAwareness;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::DEATH] = &CSpider::AI_DeadPhase;

	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::WALL] = &CSpider::AI_WallPhase;		// 적극적으로 공격
	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::FLOOR] = &CSpider::AI_FloorPhase;
	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::DEATH] = &CSpider::AI_DeadPhase;
	
	m_ePhase = PHASE::WALL;

	return S_OK;
}

_uint CSpider::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::Dead)) {
		return 0;
	}

	if (GetAsyncKeyState('6') & 0x8000)
	{
		m_bTest = true;
		m_vLook = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		m_vLook.y = 0.f;
		m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	}

	if (false == m_bTest
		&& m_pTransformCom->m_TransformDesc.vPosition.y > 20)
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
	if(PHASE::FLOOR ==  m_ePhase)
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

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(Set_Texture()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	_CollisionComp->DebugDraw();

	return S_OK;
}

void CSpider::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	// 피해를 받지 않는 상태임
	if (m_byMonsterFlag & static_cast<BYTE>(MonsterFlag::HPLock)) {
		return;
	}

	CMonster::Hit(_Target, _CollisionInfo);		// CMonster 에서 HP 감소

												// 충돌 관련 정보
	//m_vCollisionDir = _CollisionInfo.Dir;
	//m_fCrossValue = _CollisionInfo.CrossValue;

	CMonster::CreateBlood();
}

void CSpider::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	//바닥이랑 충돌하면 각도를 정면 보게
	if (m_bTest)
	{
		m_bTest = false;
		m_ePhase = PHASE::FLOOR;
	}
	else
		m_pTransformCom->m_TransformDesc.vPosition.y += 0.5f;
}

void CSpider::Update_AI(float fDeltaTime)
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

		if (m_stStatus.fHP <= 0)
		{
			m_ePhase = PHASE::DEATH;
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
	if (m_fChangeSpeed >= 1)
	{
		m_fChangeSpeed = 0.f;
		float fRandomY = rand() % 3 - 1;
		float fRandomZ = rand() % 3 - 1;
		m_vRandomLook = { 0.f,fRandomY,fRandomZ };
	}
}

void CSpider::Jump(float fDeltaTime)
{
	//if (m_pTransformCom->m_TransformDesc.vRotation.z < 0)
	//	m_pTransformCom->m_TransformDesc.vRotation.z += 3.f;
	//if (m_pTransformCom->m_TransformDesc.vRotation.z >= 0)
	//{
	//	m_pTransformCom->m_TransformDesc.vRotation.z = 0.f;
	//}
	m_fTest += fDeltaTime * 1.f;
	float Test = MATH::Lerp(m_pTransformCom->m_TransformDesc.vRotation.z, 0.f, m_fTest);
	m_pTransformCom->m_TransformDesc.vRotation.z = Test;
	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);

	if (m_fJumpTime < 3.f)
		m_fJumpTime += 0.03f;
}

void CSpider::AI_NoAwareness()
{
	m_fpAction = &CSpider::Action_Idle;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Walk";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 0.f;
}

void CSpider::AI_WallPhase()
{
	if (m_bTest)
		return;
	if (m_stStatus.fHP < m_stOriginStatus.fHP * 0.5f)
	{
		m_bTest = true;
		m_vLook = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		m_vLook.y = 0.f;
		m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	}

	int iRand = rand() % 100;
	//이동
	if (0 <= iRand && iRand < 70)
	{
		m_fpAction = &CSpider::Action_Move_Wall;
		m_fCountDown = 1.f;
		m_wstrTextureKey = m_wstrBase + L"Walk";
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fFrameSpeed = 20.f;
		m_fEndFrame = 3.f;
	}

	//웹 발사
	else if (70 <= iRand && iRand < 100)
	{
		m_fpAction = &CSpider::Action_Shoot;
		m_fCountDown = 1.f;
		m_wstrTextureKey = m_wstrBase + L"Attack";
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fFrameSpeed = 5.f;
		m_fEndFrame = 4.f;
	}

	//점프
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
	m_stStatus.fSpeed = 10.f;
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

bool CSpider::Action_Move_Wall(float fDeltaTime)
{
	//const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	//const _vector vGlacierPos = m_pTransformCom->m_TransformDesc.vPosition;
	//_vector vLook = vPlayerPos - vGlacierPos;
	//vLook.x = 0.f;
	//float fLookLength = D3DXVec3Length(&vLook);
	//D3DXVec3Normalize(&vLook, &vLook);


	//if (fLookLength < 10)
	//{
	//	m_stStatus.fHP = 40.f;
	//	return true;
	//}
	//if (fLookLength > 10)
	//	m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * m_stStatus.fSpeed;

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

	if (m_fCountDown <= 0)
	{
		return true;
	}


	return false;
}

bool CSpider::Action_Idle(float fDeltaTime)
{
	if (m_fCountDown <= 0)
	{
		return true;
	}
	return false;
}

bool CSpider::Action_Shoot(float fDeltaTime)
{
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
		CMonster::CreateFloorBlood();
	}

	return false;
}

void CSpider::CreateBullet()
{

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
	_Info.bMapCollision = true;
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
