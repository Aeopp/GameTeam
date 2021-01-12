#include "stdafx.h"
#include "..\Headers\HellBossTentacle.h"


CHellBossTentacle::CHellBossTentacle(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
	, bRelayFlag(false)
{
}


HRESULT CHellBossTentacle::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossTentacle::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 8.f,8.f,8.f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	return S_OK;
}

_uint CHellBossTentacle::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);
	Bullet_Attack(m_stStatus.fATK);	// 플레이어 충돌 처리

	
	// 전달 횟수가 남아있으면
	if (m_uiCountRelay > 0 && !bRelayFlag && m_fFrameCnt >= 2.f) {
		--m_uiCountRelay;
		bRelayFlag = true;

		// 총알 생성
		BulletBasicArgument* pArg = new BulletBasicArgument;
		pArg->uiSize = sizeof(BulletBasicArgument);
		pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition + m_vLook * 8.f;	// 생성 위치
		pArg->vDir = m_vLook;		// 방향
		pArg->uiCountRelay = m_uiCountRelay;	// 남은 카운트 전달
		m_pManagement->AddScheduledGameObjectInLayer(
			(_int)ESceneID::Static,
			L"GameObject_HellBossTentacle",
			L"Layer_Bullet",
			nullptr, (void*)pArg);
	}

	return _uint();
}

_uint CHellBossTentacle::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossTentacle::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CHellBossTentacle::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fEndFrame - 1.f;
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}
}

HRESULT CHellBossTentacle::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	// 텍스처
#pragma region Add_Component_Texture
	int iRand = rand() % 3;

	switch (iRand)
	{
	case 0:
		// 텍스처 프레임
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 10;
		m_fFrameSpeed = 20.f;

		// 몰락한 군주 가시 1
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_HellBoss_FallenLord_Spike1",
			L"Com_Texture_FallenLord_Spike1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	case 1:
		// 텍스처 프레임
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 11;
		m_fFrameSpeed = 20.f;

		// 몰락한 군주 가시 1
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_HellBoss_FallenLord_Spike2",
			L"Com_Texture_FallenLord_Spike1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	case 2:
		// 텍스처 프레임
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 18;
		m_fFrameSpeed = 20.f;

		// 몰락한 군주 가시 1
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_HellBoss_FallenLord_Tentacle",
			L"Com_Texture_FallenLord_Spike1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	}
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

CHellBossTentacle* CHellBossTentacle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossTentacle* pInstance = new CHellBossTentacle(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossTentacle");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossTentacle::Clone(void* pArg/* = nullptr*/)
{
	CHellBossTentacle* pClone = new CHellBossTentacle(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossTentacle");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossTentacle::Free()
{
	CBullet::Free();
}
