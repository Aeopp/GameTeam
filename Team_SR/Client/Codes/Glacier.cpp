#include "stdafx.h"
#include "..\Headers\Glacier.h"
#include "CollisionComponent.h"
#include "Camera.h"
#include "GlacierBullet.h"
CGlacier::CGlacier(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}


HRESULT CGlacier::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CGlacier>();

	return S_OK;
}

HRESULT CGlacier::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = m_wstrBase + L"Move";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 15;

	m_stOriginStatus.fHP = 100.f;
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
	if (true == m_bDead)
		return 0;
	CMonster::UpdateGameObject(fDeltaTime);

	//테스트
	if (GetAsyncKeyState('4') & 0x8000)
		m_stStatus.fHP -= 1;

	//테스트
	Update_AI(fDeltaTime);

	cout << m_stStatus.fHP << endl;

	IsBillboarding();

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CGlacier::LateUpdateGameObject(float fDeltaTime)
{
	if (true == m_bDead)
	{
		if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
			return 0;
		return 0;
	}
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (AWARENESS::Yes == m_eAwareness)
		m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CGlacier::RenderGameObject()
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
	_Info.Radius = 2.5f;
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


HRESULT CGlacier::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	_matrix matBillboardY, matView;
	D3DXMatrixIdentity(&matBillboardY);
	matView = pCamera->GetCameraDesc().matView;

	matBillboardY._11 = matView._11;
	matBillboardY._13 = matView._13;
	matBillboardY._31 = matView._31;
	matBillboardY._33 = matView._33;

	D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

	m_pTransformCom->m_TransformDesc.matWorld *= matBillboardY;

	

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
	//빌보드로 플레이어 바라보기
	m_fpAction = &CGlacier::Action_Move;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Move";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 15.f;
}

void CGlacier::AI_SecondPhase()
{
	//총알 발사

	m_fpAction = &CGlacier::Action_Hurt;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Hurt";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 15.f;

}

void CGlacier::AI_ThirdPhase()
{
	//총알 발사
	m_fpAction = &CGlacier::Action_Shoot;
	m_fCountDown = 1.f;
	m_wstrTextureKey = m_wstrBase + L"Attack";
	m_fFrameCnt = 0.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 10.f;
}

void CGlacier::AI_DeadPhase()
{
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
		m_bDead = true;
		m_fFrameCnt = 8.f;
	}
	return false;
}

void CGlacier::CreateBullet()
{
	_vector pPositionArr[2];
	pPositionArr[0] = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	pPositionArr[1] = m_pTransformCom->m_TransformDesc.vPosition;
	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CGlacierBullet>(),
		(_int)ESceneID::Stage1st,
		CGameObject::Tag + TYPE_NAME<CGlacierBullet>(),
		nullptr, (void*)pPositionArr)))
		return;


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
	for (auto& rPair : m_mapTexture)
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	SafeRelease(_CollisionComp);
	SafeRelease(m_pVIBufferCom);

	CGameObject::Free();
}
