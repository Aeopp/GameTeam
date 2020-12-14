#include "stdafx.h"
#include "..\Headers\BatGrey.h"


CBatGrey::CBatGrey(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
	, m_fCountdown(0.f), m_fpAction(nullptr)
	, m_eAwareness(AWARENESS::End), m_ePhase(PHASE::End)
	, m_fpMonsterAI{}
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

	// 몬스터 원본 스텟
	m_stOriginStatus.fHP = 100.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fDEF = 0.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fDetectionDistance = 50.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_wstrTextureKey = L"Component_Texture_BatGreyFly";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 8;

	// 기본 대기 행동
	m_fpAction = &CBatGrey::Action_Idle;
	// 플레이어를 인식하지 못함
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Full] = &CBatGrey::AI_NoAwareness;
	m_fpMonsterAI[(int)AWARENESS::No][(int)PHASE::HP_Half] = &CBatGrey::AI_NoAwareness;

	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Full] = &CBatGrey::AI_ActiveOffense;		// 적극적으로 공격
	m_fpMonsterAI[(int)AWARENESS::Yes][(int)PHASE::HP_Half] = &CBatGrey::AI_PassiveOffense;		// 소극적으로 공격

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

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

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
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 텍스처
	CTexture* pTexture;
	// 플라이
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyFly",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyFly", pTexture);

	// 원거리 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyShoot",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyShoot", pTexture);

	// 근접 공격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyAttack",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyAttack", pTexture);

	// 뒤돌아봄
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyBack",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyBack", pTexture);

	// 피격
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyHit",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyHit", pTexture);

	// 죽음
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_BatGreyDeath",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_BatGreyDeath", pTexture);
#pragma endregion

	return S_OK;
}

// AI는 하나의 행동을 끝마친 후에 새로운 행동을 받는다
void CBatGrey::Update_AI(float fDeltaTime)
{
	// 몬스터 행동
	if ((this->*m_fpAction)(fDeltaTime)) {
		// 플레이어를 인식했는가?
		if (PlayerAwareness()) {
			m_eAwareness = AWARENESS::Yes;	// 플레이어 발견
		}
		else {
			m_eAwareness = AWARENESS::No;	// 플레이어 몰라
		}

		// 체력에 따른 페이즈
		if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.5f) {
			m_ePhase = PHASE::HP_Full;	// 체력이 절반 이상
		}
		else {
			m_ePhase = PHASE::HP_Half;	// 체력이 절반 이하
		}

		// AI 처리
		(this->*m_fpMonsterAI[(int)m_eAwareness][(int)m_ePhase])();
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

// 플레이어를 인식하지 못함
void CBatGrey::AI_NoAwareness()
{
	m_fpAction = &CBatGrey::Action_Idle;	// 대기
	m_fCountdown = 1.f;		// 1초
	// 텍스처 키, 프레임
	m_wstrTextureKey = L"Component_Texture_BatGreyFly";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 8;
}

// 적극적으로 공격
void CBatGrey::AI_ActiveOffense()
{
	// 랜덤으로 몇가지 패턴 행동을...

	{
		m_fpAction = &CBatGrey::Action_Shoot;
		m_wstrTextureKey = L"Component_Texture_BatGreyShoot";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 5;
	}
}

// 소극적으로 공격
void CBatGrey::AI_PassiveOffense()
{
}

// 행동 대기
bool CBatGrey::Action_Idle(float fDeltaTime)
{
	m_fCountdown -= fDeltaTime;
	if (m_fCountdown <= 0) {
		return true;
	}
	return false;
}

// 원거리 공격
bool CBatGrey::Action_Shoot(float fDeltaTime)
{
	if (m_bFrameLoopCheck) {
		return true;
	}

	return false;
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
	for (auto& rPair : m_mapTexture)
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	SafeRelease(m_pVIBufferCom);

	CMonster::Free();
}
