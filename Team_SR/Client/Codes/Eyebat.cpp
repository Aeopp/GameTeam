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

	m_stOriginStatus.fHP = 100.f;
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

	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_High] = &CEyebat::AI_FirstPhase;		// ���������� ����
	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_Low] = &CEyebat::AI_SecondPhase;	// �ұ������� ����
	m_fpEyebatAI[(int)AWARENESS::Yes][(int)PHASE::HP_ZERO] = &CEyebat::AI_DeadPhase;	// �ұ������� ����

	return S_OK;
}

_uint CEyebat::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	//�׽�Ʈ
	if (GetAsyncKeyState('5') & 0x8000)
		m_stStatus.fHP -= 1.f;

	Update_AI(fDeltaTime);


	return _uint();
}

_uint CEyebat::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (AWARENESS::Yes == m_eAwareness)
		m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;


	return _uint();
}

HRESULT CEyebat::RenderGameObject()
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

void CEyebat::Update_AI(float fDeltaTime)
{
	if ((this->*m_fpAction)(fDeltaTime))
	{
		// �÷��̾ �ν��ߴ°�?
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

	if (fLookLength > 10)
		m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * m_stStatus.fSpeed;

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
	return false;
}

void CEyebat::CreateBullet()
{
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
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp���� RectTexture ȣ��
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

	return S_OK;
}

HRESULT CEyebat::Set_Texture()
{

	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// �ش� ������ �ؽ�ó ��ġ�� ��
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
	CEyebat* pClone = new CEyebat(*this); /* ��������� */
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
	//SafeRelease(_CollisionComp);

	CMonster::Free();
}
