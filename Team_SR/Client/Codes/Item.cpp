#include "stdafx.h"
#include "..\Headers\Item.h"
#include "Camera.h"

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr), m_stItemInfo{}
{
}

HRESULT CItem::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (sizeof(ItemBasicArgument) == *static_cast<_uint*>(pArg)) {
			ItemBasicArgument* pArgument = static_cast<ItemBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_stItemInfo.etype = pArgument->etype;
			// 동적 생성된 거임
			if (pArgument->bDeleteFlag) {
				delete pArg;
			}
		}
	}

	m_pTransformCom->m_TransformDesc.vScale = { 0.5f, 0.5f, 0.5f };

	if (FAILED(CItem::AddComponents()))
		return E_FAIL;

	return S_OK;
}

_uint CItem::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CItem::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);
	IsBillboarding();

	return _uint();
}

HRESULT CItem::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Monster;
	_Info.bMapCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

#pragma region Item_Texture
	// 아이템 타입으로 텍스처 컴포넌트 추가
	switch (m_stItemInfo.etype)
	{
		// 겁나큰 체력 포션 텍스처
	case ITEM::HealthBig:
		m_stItemInfo.iAmount = 50;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Health_Big",
			L"Com_Texture_Health_Big",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 작은 체력 포션 텍스처
	case ITEM::HealthSmall:
		m_stItemInfo.iAmount = 20;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Health_Small",
			L"Com_Texture_Health_Small",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 겁나큰 마나 포션 텍스처
	case ITEM::ManaBig:
		m_stItemInfo.iAmount = 50;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Mana_Big",
			L"Com_Texture_Mana_Big",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 작은 마나 포션 텍스처
	case ITEM::ManaSmall:
		m_stItemInfo.iAmount = 20;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 4;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Mana_Small",
			L"Com_Texture_Mana_Small",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 총알 박스
	case ITEM::Ammo:
		m_stItemInfo.iAmount = 30;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 0;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Ammo_Box",
			L"Com_Texture_Ammo_Box",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 파란 열쇠
	case ITEM::KeyBlue:
		m_stItemInfo.iAmount = 1;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 0;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Key_Blue",
			L"Com_Texture_Key_Blue",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 빨강 열쇠
	case ITEM::KeyRed:
		m_stItemInfo.iAmount = 1;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 0;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Key_Blue",
			L"Com_Texture_Key_Blue",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 노랑 열쇠
	case ITEM::KeyYellow:
		m_stItemInfo.iAmount = 1;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 0;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Key_Yellow",
			L"Com_Texture_Key_Yellow",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 업그레이드 재화
	case ITEM::Upgrade:
		m_stItemInfo.iAmount = 1;
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 0;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Upgrade",
			L"Com_Texture_Upgrade",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	default:
		// 예약되지 않은 타입
		return E_FAIL;
		break;
	}
#pragma endregion	// Item_Texture

	return S_OK;
}

// 아이템 정보
ItemInfo CItem::GetItemInfo()
{
	return m_stItemInfo;
}

// 텍스처 프레임 이동
void CItem::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

HRESULT CItem::IsBillboarding()
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

CItem * CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CItem* pInstance = new CItem(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CItem");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem* pClone = new CItem(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CItem");
		SafeRelease(pClone);
	}

	return pClone;
}

void CItem::Free()
{
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}


