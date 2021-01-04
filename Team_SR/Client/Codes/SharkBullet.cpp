#include "stdafx.h"
#include "..\Headers\SharkBullet.h"

CSharkBullet::CSharkBullet(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}

HRESULT CSharkBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;
	m_wstrBase = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CSharkBullet>();
	return S_OK;
}

HRESULT CSharkBullet::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
	}
	m_pTransformCom->m_TransformDesc.vPosition.y = 8.f;
	m_pTransformCom->m_TransformDesc.vScale = { 5.f, 5.f, 5.f };
	int random = rand() % 3;
	bGravity = false;
	switch (random)
	{
	case 0:
		m_wstrTextureKey = m_wstrBase + L"icicleA";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 17.f;
		m_fFrameSpeed = 10.f;
		break;
	case 1:
		m_wstrTextureKey = m_wstrBase + L"icicleB";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 15.f;
		m_fFrameSpeed = 10.f;
		break;
	case 2:
		m_wstrTextureKey = m_wstrBase + L"icicleC";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 17.f;
		m_fFrameSpeed = 10.f;
		break;
	default:
		m_wstrTextureKey = m_wstrBase + L"icicleA";
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 17.f;
		m_fFrameSpeed = 10.f;
		break;
	}

	return S_OK;
}

_uint CSharkBullet::UpdateGameObject(float fDeltaTime)
{
	if (m_bFrameLoopCheck)
		m_byObjFlag |= (BYTE)ObjFlag::Remove;
	if (m_byObjFlag & (BYTE)ObjFlag::Remove)
		return 0;
	CMonster::UpdateGameObject(fDeltaTime);

	if (!m_bTest)
	{
		if (CMonster::Attack(_CollisionComp->_Sphere, 10.f))
		{
			m_bTest = true;
		}
	}
	_CollisionComp->Update(m_pTransformCom);
	return _uint();
}

_uint CSharkBullet::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);



	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	return _uint();
}

HRESULT CSharkBullet::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

void CSharkBullet::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
}

HRESULT CSharkBullet::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))
		return E_FAIL;

	CTexture* pTexture = nullptr;
	
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"icicleA",
		L"Com_Texture_icicleA",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"icicleA", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"icicleB",
		L"Com_Texture_icicleB",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"icicleB", pTexture);

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrBase + L"icicleC",
		L"Com_Texture_icicleC",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrBase + L"icicleC", pTexture);


	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 3.0f;
	_Info.Tag = CCollisionComponent::ETag::Monster;
	_Info.bFloorCollision = false;
	_Info.bWallCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

HRESULT CSharkBullet::Set_Texture()
{
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

CSharkBullet * CSharkBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSharkBullet* pInstance = new CSharkBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CSharkBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CSharkBullet::Clone(void * pArg /*= nullptr*/)
{
	CSharkBullet* pClone = new CSharkBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CSharkBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CSharkBullet::Free()
{
	CMonster::Free();
}
