#include "stdafx.h"
#include "..\Headers\HangmanBomb.h"


CHangmanBomb::CHangmanBomb(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}

HRESULT CHangmanBomb::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHangmanBomb::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_vLook.y = 0.f;

	m_pTransformCom->m_TransformDesc.vScale = { 1.f, 1.f, 1.f };

	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 100;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 1.f;
	m_stOriginStatus.fImpact = 0.f;
	m_stStatus = m_stOriginStatus;

	m_fJumpPower = 15.f;
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;

	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 0;

	return S_OK;
}

_uint CHangmanBomb::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	// 포물선 이동
	Movement(fDeltaTime);

	if (m_pTransformCom->m_TransformDesc.vPosition.y < 0) {
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
		CreateFire();
	}
	
	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHangmanBomb::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CHangmanBomb::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	return S_OK;
}

HRESULT CHangmanBomb::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Hangman_Bullet",
		L"Com_Texture_Hangman_Bullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 2.5f;
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

// 포물선 이동
void CHangmanBomb::Movement(float fDeltaTime)
{
	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);
	m_fJumpTime += fDeltaTime;
	
	m_pTransformCom->m_TransformDesc.vRotation.z += 3.f;
}

void CHangmanBomb::CreateFire()
{
	//if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
	//	CGameObject::Tag + TYPE_NAME<CFire>(),
	//	(_int)ESceneID::Stage1st,
	//	CGameObject::Tag + TYPE_NAME<CFire>(),
	//	nullptr, (void*)&m_pTransformCom->m_TransformDesc.vPosition)))
	//	return;
	//m_bTest = true;
}

CHangmanBomb * CHangmanBomb::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHangmanBomb* pInstance = new CHangmanBomb(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHangmanBomb");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CHangmanBomb::Clone(void * pArg /*= nullptr*/)
{
	CHangmanBomb* pClone = new CHangmanBomb(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHangmanBomb");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHangmanBomb::Free()
{
	CBullet::Free();
}
