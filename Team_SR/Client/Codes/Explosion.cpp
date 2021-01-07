#include "stdafx.h"
#include "..\Headers\Explosion.h"


CExplosion::CExplosion(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
	, bRelayFlag(false)
{
}


HRESULT CExplosion::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExplosion::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 10.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 13;
	m_fFrameSpeed = 20.f;

	return S_OK;
}

_uint CExplosion::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);
	Bullet_Attack(m_stStatus.fATK);	// 플레이어 충돌 처리

	return _uint();
}

_uint CExplosion::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CExplosion::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CExplosion::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	// Explosion은 맵과 부딪혀도 삭제되지않음
	// Bullet 함수 쪽의 가상 함수 MapHit에서 삭제되는 처리를 막기 위함
	// 선언만 해둠
}

void CExplosion::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fEndFrame - 1.f;
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}
}

HRESULT CExplosion::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	// 텍스처
#pragma region Add_Component_Texture
	int iRand = rand() % 3;

	switch (iRand)
	{
	case 0:
		// 폭발 0
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Explosion0",
			L"Com_Texture_Explosion0",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	case 1:
		// 폭발 1
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Explosion1",
			L"Com_Texture_Explosion2",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	case 2:
		// 폭발 2
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Explosion2",
			L"Com_Texture_Explosion3",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	}
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 2.f;
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

CExplosion* CExplosion::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CExplosion* pInstance = new CExplosion(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CExplosion");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion::Clone(void* pArg/* = nullptr*/)
{
	CExplosion* pClone = new CExplosion(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CExplosion");
		SafeRelease(pClone);
	}

	return pClone;
}

void CExplosion::Free()
{
	CBullet::Free();
}
