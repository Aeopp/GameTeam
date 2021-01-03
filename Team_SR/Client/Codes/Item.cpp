#include "stdafx.h"
#include "..\Headers\Item.h"
#include "MainCamera.h"
#include "DXWrapper.h"
#include "NormalUVVertexBuffer.h"


CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr), m_stItemInfo{}{}

HRESULT CItem::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	bGravity = true;

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
	CreateAfterTime = 0.0f;

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

	CreateAfterTime += fDeltaTime;
	if (CreateAfterTime >= 10.0f)
	{
		bGravity = false;
		_CollisionComp->bFloorCollision = false;
		_CollisionComp->bWallCollision = false;
	}

	return _uint();
}

HRESULT CItem::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), m_pTexture->GetTexture((_uint)m_fFrameCnt));
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
		}
		// 2. 세팅을 하고 난 이후의                                   ↑↑↑↑↑↑↑↑↑↑     TRUE 로 바꾸어주기.
		{
			// m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),SpecularTexture);
			// m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),NormalTexture);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", 20.f);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();

	return S_OK;
}

HRESULT CItem::AddComponents()
{


	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;

	_Info.bCollision = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Item;
	_Info.bWallCollision = false;
	_Info.bFloorCollision = true;
	_Info.bMapBlock = true;

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


	CMainCamera* pCamera = dynamic_cast<CMainCamera*> (m_pManagement->GetGameObject((_int)-1, L"Layer_MainCamera"));
	if (nullptr == pCamera)
		return E_FAIL;

	if (pCamera->bThirdPerson)
	{
			_matrix matBillboardY, matView;
		D3DXMatrixIdentity(&matBillboardY);
		matView = pCamera->GetCameraDesc().matView;

		matBillboardY._11 = matView._11;
		matBillboardY._13 = matView._13;
		matBillboardY._31 = matView._31;
		matBillboardY._33 = matView._33;

		D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

		m_pTransformCom->m_TransformDesc.matWorld *= matBillboardY;
	}
	else
	{
		const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;
		vec3 BillboardRotation = _TransformDesc.vRotation;
		BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
		m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);
	}


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
	SafeRelease(m_pTexture);		// 텍스처
		/// <summary> 2020 12 20 이호준
	SafeRelease(_VertexBuffer);
	/// </summary>
	CGameObject::Free();
}

void CItem::Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo)
{
	bAcheive = true;
	bGravity = false;
	_CollisionComp->bCollision = false;
	_CollisionComp->bFloorCollision = false;
	_CollisionComp->bWallCollision = false;

	m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);
}


