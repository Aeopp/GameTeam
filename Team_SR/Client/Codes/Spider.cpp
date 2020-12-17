#include "stdafx.h"
#include "..\Headers\Spider.h"


CSpider::CSpider(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}

HRESULT CSpider::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CSpider::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_fpAction = &CSpider::Action_Idle;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::HP_High] = &CSpider::AI_NoAwareness;
	m_fpSpiderAI[(int)AWARENESS::No][(int)PHASE::HP_ZERO] = &CSpider::AI_NoAwareness;

	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::HP_High] = &CSpider::AI_FirstPhase;		// 적극적으로 공격
	m_fpSpiderAI[(int)AWARENESS::Yes][(int)PHASE::HP_ZERO] = &CSpider::AI_DeadPhase;	// 소극적으로 공격


	return S_OK;
}

_uint CSpider::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	Update_AI(fDeltaTime);
	return _uint();
}

_uint CSpider::LateUpdateGameObject(float fDeltaTime)
{

	CMonster::LateUpdateGameObject(fDeltaTime);

	if (AWARENESS::Yes == m_eAwareness)
		m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;


	return _uint();
}

HRESULT CSpider::RenderGameObject()
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

		if (m_stStatus.fHP > 0) {
			m_ePhase = PHASE::HP_High;
		}
		else if (m_stStatus.fHP <= 0)
		{
			m_ePhase = PHASE::HP_ZERO;
		}

		(this->*m_fpSpiderAI[(int)m_eAwareness][(int)m_ePhase])();
	}

}

void CSpider::AI_NoAwareness()
{
}

void CSpider::AI_FirstPhase()
{
}

void CSpider::AI_DeadPhase()
{
}

bool CSpider::Action_Move(float fDeltaTime)
{
	return false;
}

bool CSpider::Action_Idle(float fDeltaTime)
{
	return false;
}

bool CSpider::Action_Shoot(float fDeltaTime)
{
	return false;
}

bool CSpider::Action_Death(float fDeltaTime)
{
	return false;
}

HRESULT CSpider::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;


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
