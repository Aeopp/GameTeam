#include "stdafx.h"
#include "..\Headers\Glacier.h"
#include "CollisionComponent.h"
#include "Camera.h"

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
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_Full] = &CGlacier::AI_NoAwareness;
	m_fpGlacierAI[(int)AWARENESS::No][(int)PHASE::HP_Half] = &CGlacier::AI_NoAwareness;

	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_Full] = &CGlacier::AI_FirstPhase;		// 적극적으로 공격
	m_fpGlacierAI[(int)AWARENESS::Yes][(int)PHASE::HP_Half] = &CGlacier::AI_SecondPhase;		// 소극적으로 공격

	
	return S_OK;
}

_uint CGlacier::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	//테스트
	if (GetAsyncKeyState('4'))
		m_stStatus.fHP -= 5;

	//테스트
	Update_AI(fDeltaTime);

	IsBillboarding();

	return _uint();
}

_uint CGlacier::LateUpdateGameObject(float fDeltaTime)
{
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
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Move", pTexture);
	// Death
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Death",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Death", pTexture);
	// Attack
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Attack",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Attack", pTexture);
	// Hurt
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"Hurt",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"Hurt", pTexture);

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


		if (m_stStatus.fHP > m_stOriginStatus.fHP * 0.5f) {
			m_ePhase = PHASE::HP_Full;	
		}
		else {
			m_ePhase = PHASE::HP_Half;	
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

bool CGlacier::Action_Move(float fDeltaTime)
{
	const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	const _vector vGlacierPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vLook = vPlayerPos - vGlacierPos;
	float fPlayerYDegree = m_pPlayer->GetTransform()->m_TransformDesc.vRotation.y;
	float fLookLength = D3DXVec3Length(&vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if ((fLookLength > 5 && (fLookLength < 60 || true == m_bHit)))
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
	return false;
}

bool CGlacier::Action_Hurt(float fDeltaTime)
{
	return false;
}

bool CGlacier::Action_Death(float fDeltaTime)
{
	return false;
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

	SafeRelease(m_pVIBufferCom);

	CGameObject::Free();
}
