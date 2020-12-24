#include "stdafx.h"
#include "..\Headers\Decorator.h"
#include "MainCamera.h"
#include "DXWrapper.h"
#include "NormalUVVertexBuffer.h"

CDecorator::CDecorator(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr), m_stDecoratorInfo{}
{
}

HRESULT CDecorator::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	bGravity = true;

	return S_OK;
}

HRESULT CDecorator::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (sizeof(DecoratorBasicArgument) == *static_cast<_uint*>(pArg)) {
			DecoratorBasicArgument* pArgument = static_cast<DecoratorBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_stDecoratorInfo.eType = pArgument->eType;
			// 동적 생성된 거임
			if (pArgument->bDeleteFlag) {
				delete pArg;
			}
		}
	}

	if (FAILED(CDecorator::AddComponents()))
		return E_FAIL;

	return S_OK;
}

_uint CDecorator::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);

	UpdateFromMyDecoType();

	return _uint();
}

_uint CDecorator::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);
	IsBillboarding();

	return _uint();
}

HRESULT CDecorator::RenderGameObject()
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
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", false);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", false);
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

HRESULT CDecorator::AddComponents()
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
	_Info.Tag = CCollisionComponent::ETag::Decorator;
	_Info.bWallCollision = false;
	_Info.bFloorCollision = true;
	_Info.bMapBlock = true;
	_Info.Owner = this;
	
	DecoNextFrameInfo NextFrameInfo;

#pragma region Item_Texture
	// 아이템 타입으로 텍스처 컴포넌트 추가
	switch (m_stDecoratorInfo.eType)
	{
		// 횃불
	case DECO::Torch:
		m_pTransformCom->m_TransformDesc.vScale = { 1.f, 3.f, 1.f };
		_Info.Radius = 1.5f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 1.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 8.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 8.f;
		NextFrameInfo.fEndFrame = 9.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Torch",
			L"Com_Texture_Torch",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 양초
	case DECO::Candle:
		m_pTransformCom->m_TransformDesc.vScale = { 1.f, 3.5f, 1.f };
		_Info.Radius = 1.75f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 5.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Candle",
			L"Com_Texture_Candle",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 나무 통
	case DECO::Barrel:
		m_pTransformCom->m_TransformDesc.vScale = { 2.f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 10.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Barrel",
			L"Com_Texture_Barrel",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 폭탄 통
	case DECO::BarrelBomb:
		m_pTransformCom->m_TransformDesc.vScale = { 2.f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 10.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_BarrelBomb",
			L"Com_Texture_BarrelBomb",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 폐기물 통
	case DECO::BarrelWaste:
		m_pTransformCom->m_TransformDesc.vScale = { 2.f, 2.2f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 100.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 50.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 0.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_BarrelWaste",
			L"Com_Texture_BarrelWaste",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 나무 상자 1
	case DECO::Box1:
		m_pTransformCom->m_TransformDesc.vScale = { 2.8f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 10.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Box1",
			L"Com_Texture_Box1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 나무 상자 2
	case DECO::Box2:
		m_pTransformCom->m_TransformDesc.vScale = { 2.8f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 10.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Box2",
			L"Com_Texture_Box2",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 강철 상자 1
	case DECO::BoxSteel1:
		m_pTransformCom->m_TransformDesc.vScale = { 2.8f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 100.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_BoxSteel1",
			L"Com_Texture_BoxSteel1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 강철 상자 2
	case DECO::BoxSteel2:
		m_pTransformCom->m_TransformDesc.vScale = { 2.8f, 2.f, 1.f };
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 100.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 0.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_BoxSteel2",
			L"Com_Texture_BoxSteel2",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 묘비 1
	case DECO::Headstone1:
		m_pTransformCom->m_TransformDesc.vScale = { 4.f, 4.f, 1.f };
		_Info.Radius = 2.f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 700.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 650.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 550.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = 450.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 3.f;
		NextFrameInfo.fEndFrame = 4.f;
		NextFrameInfo.fTriggerHP = 350.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 4.f;
		NextFrameInfo.fEndFrame = 5.f;
		NextFrameInfo.fTriggerHP = 250.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 5.f;
		NextFrameInfo.fEndFrame = 6.f;
		NextFrameInfo.fTriggerHP = 150.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 6.f;
		NextFrameInfo.fEndFrame = 7.f;
		NextFrameInfo.fTriggerHP = 100.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 7.f;
		NextFrameInfo.fEndFrame = 8.f;
		NextFrameInfo.fTriggerHP = 50.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 8.f;
		NextFrameInfo.fEndFrame = 9.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Headstone1",
			L"Com_Texture_Headstone1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 묘비 2
	case DECO::Headstone2:
		m_pTransformCom->m_TransformDesc.vScale = { 4.f, 4.f, 1.f };
		_Info.Radius = 2.f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 700.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 650.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 550.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = 450.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 3.f;
		NextFrameInfo.fEndFrame = 4.f;
		NextFrameInfo.fTriggerHP = 350.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 4.f;
		NextFrameInfo.fEndFrame = 5.f;
		NextFrameInfo.fTriggerHP = 250.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 5.f;
		NextFrameInfo.fEndFrame = 6.f;
		NextFrameInfo.fTriggerHP = 150.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 6.f;
		NextFrameInfo.fEndFrame = 7.f;
		NextFrameInfo.fTriggerHP = 100.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 7.f;
		NextFrameInfo.fEndFrame = 8.f;
		NextFrameInfo.fTriggerHP = 50.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 8.f;
		NextFrameInfo.fEndFrame = 9.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Headstone2",
			L"Com_Texture_Headstone2",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 묘비 3
	case DECO::Headstone3:
		m_pTransformCom->m_TransformDesc.vScale = { 4.f, 4.f, 1.f };
		_Info.Radius = 2.f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 600.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 500.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 400.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = 300.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 3.f;
		NextFrameInfo.fEndFrame = 4.f;
		NextFrameInfo.fTriggerHP = 200.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 4.f;
		NextFrameInfo.fEndFrame = 5.f;
		NextFrameInfo.fTriggerHP = 100.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 5.f;
		NextFrameInfo.fEndFrame = 6.f;
		NextFrameInfo.fTriggerHP = 50.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 6.f;
		NextFrameInfo.fEndFrame = 7.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Headstone3",
			L"Com_Texture_Headstone3",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 가시 덩굴
	case DECO::ThornyVine:
		m_pTransformCom->m_TransformDesc.vScale = { 4.f, 4.f, 1.f };
		_Info.Radius = 2.f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 500.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 400.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 300.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = 200.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 3.f;
		NextFrameInfo.fEndFrame = 4.f;
		NextFrameInfo.fTriggerHP = 100.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 4.f;
		NextFrameInfo.fEndFrame = 5.f;
		NextFrameInfo.fTriggerHP = 50.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 5.f;
		NextFrameInfo.fEndFrame = 6.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_ThornyVine",
			L"Com_Texture_ThornyVine",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 나무
	case DECO::TreeBlight:
		m_pTransformCom->m_TransformDesc.vScale = { 10.f, 10.f, 1.f };
		_Info.Radius = 5.f;
		_Info.Tag = CCollisionComponent::ETag::DestroyDecorator;
		m_stDecoratorInfo.fHP = 500.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = 400.f;
		NextFrameInfo.fStartFrame = 1.f;
		NextFrameInfo.fEndFrame = 2.f;
		NextFrameInfo.fTriggerHP = 200.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 2.f;
		NextFrameInfo.fEndFrame = 3.f;
		NextFrameInfo.fTriggerHP = 100.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 3.f;
		NextFrameInfo.fEndFrame = 4.f;
		NextFrameInfo.fTriggerHP = 50.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		NextFrameInfo.fStartFrame = 4.f;
		NextFrameInfo.fEndFrame = 5.f;
		NextFrameInfo.fTriggerHP = -1.f;
		m_listNextFrameInfo.emplace_back(NextFrameInfo);
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_TreeBlight",
			L"Com_Texture_TreeBlight",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 나무 1
	case DECO::Tree1:
		m_pTransformCom->m_TransformDesc.vScale = { 10.f, 10.f, 1.f };
		_Info.Radius = 5.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Tree1",
			L"Com_Texture_Tree1",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 나무 2
	case DECO::Tree2:
		m_pTransformCom->m_TransformDesc.vScale = { 10.f, 10.f, 1.f };
		_Info.Radius = 5.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Tree2",
			L"Com_Texture_Tree2",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 시체 1
	case DECO::Dead_Caleb:
		m_pTransformCom->m_TransformDesc.vScale = { 6.f, 6.f, 1.f };
		_Info.Radius = 3.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Dead_Caleb",
			L"Com_Texture_Dead_Caleb",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 시체 2
	case DECO::Dead_Doomguy:
		m_pTransformCom->m_TransformDesc.vScale = { 6.f, 6.f, 1.f };
		_Info.Radius = 3.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Dead_Doomguy",
			L"Com_Texture_Dead_Doomguy",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 시체 3
	case DECO::Dead_Duke:
		m_pTransformCom->m_TransformDesc.vScale = { 6.f, 6.f, 1.f };
		_Info.Radius = 3.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Dead_Duke",
			L"Com_Texture_Dead_Duke",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 시체 4
	case DECO::Dead_Sam:
		m_pTransformCom->m_TransformDesc.vScale = { 6.f, 6.f, 1.f };
		_Info.Radius = 3.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Dead_Sam",
			L"Com_Texture_Dead_Sam",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
		// 죽은 시체 5
	case DECO::Dead_Wang:
		m_pTransformCom->m_TransformDesc.vScale = { 6.f, 6.f, 1.f };
		_Info.Radius = 3.f;
		m_fFrameCnt = 0.f;
		m_fStartFrame = 0.f;
		m_fEndFrame = 1.f;
		m_fTriggerHP = -1.f;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_Dead_Wang",
			L"Com_Texture_Dead_Wang",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	default:
		// 예약되지 않은 타입
		return E_FAIL;
		break;
	}
#pragma endregion	// Item_Texture

	// 충돌 컴포넌트 추가
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

// 장식이 피해를 받음
void CDecorator::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	m_stDecoratorInfo.fHP -= 1.f;

	if (m_fTriggerHP != -1.f) 
	{
		while (m_stDecoratorInfo.fHP <= m_fTriggerHP && m_listNextFrameInfo.size() != 0) {
			auto iter = m_listNextFrameInfo.begin();
			m_fFrameCnt = iter->fStartFrame;
			m_fStartFrame = iter->fStartFrame;
			m_fEndFrame = iter->fEndFrame;
			m_fTriggerHP = iter->fTriggerHP;
			m_listNextFrameInfo.erase(iter);
		}
	}
}

// 텍스처 프레임 이동
void CDecorator::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

HRESULT CDecorator::IsBillboarding()
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

CDecorator * CDecorator::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CDecorator* pInstance = new CDecorator(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CDecorator");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CDecorator::Clone(void * pArg)
{
	CDecorator* pClone = new CDecorator(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CDecorator");
		SafeRelease(pClone);
	}

	return pClone;
}

void CDecorator::Free()
{
	SafeRelease(m_pTexture);		// 텍스처
		/// <summary> 2020 12 20 이호준
	SafeRelease(_VertexBuffer);
	/// </summary>
	CGameObject::Free();
}

void CDecorator::UpdateFromMyDecoType()
{
	MyLight _Light;
	_Light.Diffuse = { 1,1,1,1 };
	_Light.Location = MATH::ConvertVec4(m_pTransformCom->GetLocation(), 1.f);
	_Light.Priority = 2ul;
	_Light.Radius = 20.f;

	switch (m_stDecoratorInfo.eType)
	{
	case Decorator::Torch:
		Effect::RegistLight(std::move(_Light));
		break;
	default:
		break;
	}
}


