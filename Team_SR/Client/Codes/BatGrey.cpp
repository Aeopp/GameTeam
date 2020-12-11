#include "stdafx.h"
#include "..\Headers\BatGrey.h"


CBatGrey::CBatGrey(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}


HRESULT CBatGrey::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBatGrey::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Component_Texture_BatGreyFly";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 8;

	// 기본 대기 행동
	m_fpAction = &CBatGrey::Action_Idle;
	// 플레이어를 인식하지 못함
	m_fpMonsterAI[(int)PERCEPTION::No][(int)PHASE::HP_Full] = &CBatGrey::AI_NoPerception;
	m_fpMonsterAI[(int)PERCEPTION::No][(int)PHASE::HP_Half] = &CBatGrey::AI_NoPerception;

	m_fpMonsterAI[(int)PERCEPTION::Yes][(int)PHASE::HP_Full] = &CBatGrey::AI_ActiveOffense;		// 적극적으로 공격
	m_fpMonsterAI[(int)PERCEPTION::Yes][(int)PHASE::HP_Half] = &CBatGrey::AI_PassiveOffense;	// 소극적으로 공격

	return S_OK;
}

_uint CBatGrey::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	Update_AI(fDeltaTime);	// 업데이트 AI

	return _uint();
}

_uint CBatGrey::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CBatGrey::RenderGameObject()
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

HRESULT CBatGrey::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTexture* pTexture;
	// 텍스처 플라이
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyFly",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;

	m_mapTexture.emplace(L"Component_Texture_BatGreyFly", pTexture);

	return S_OK;
}

void CBatGrey::Update_AI(float fDeltaTime)
{
	if (m_fpAction) {
		// 인지

		// 체력

		// AI 처리
		m_fpMonsterAI[(int)PERCEPTION::End][(int)PHASE::End];
	}
}

void CBatGrey::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fEndFrame * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

HRESULT CBatGrey::Set_Texture()
{
	// 텍스처 찾기
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

void CBatGrey::AI_NoPerception()
{
}

void CBatGrey::AI_ActiveOffense()
{
}

void CBatGrey::AI_PassiveOffense()
{
}

BOOL CBatGrey::Action_Idle(float fDeltaTime)
{
	return FALSE;
}

CBatGrey* CBatGrey::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CBatGrey* pInstance = new CBatGrey(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacier");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CBatGrey::Clone(void* pArg/* = nullptr*/)
{
	CBatGrey* pClone = new CBatGrey(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CBatGrey::Free()
{
	CMonster::Free();
}
