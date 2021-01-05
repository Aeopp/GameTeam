#include "stdafx.h"
#include "..\Headers\HellBossChainGunBullet.h"


CHellBossChainGunBullet::CHellBossChainGunBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CHellBossChainGunBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossChainGunBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 1.f,1.f,1.f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 30.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;

	return S_OK;
}

_uint CHellBossChainGunBullet::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHellBossChainGunBullet::LateUpdateGameObject(float fDeltaTime)
{
	//CBullet::LateUpdateGameObject(fDeltaTime);	// 빌보드 X

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossChainGunBullet::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossChainGunBullet::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 터보 사탄 체인건 총알 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_ChainGunBullet",
		L"Com_Texture_TurboSatan_ChainGunBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
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

CHellBossChainGunBullet* CHellBossChainGunBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossChainGunBullet* pInstance = new CHellBossChainGunBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossChainGunBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossChainGunBullet::Clone(void* pArg/* = nullptr*/)
{
	CHellBossChainGunBullet* pClone = new CHellBossChainGunBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossChainGunBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossChainGunBullet::Free()
{
	CBullet::Free();
}