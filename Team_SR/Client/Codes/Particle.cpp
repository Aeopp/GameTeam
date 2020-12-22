#include "stdafx.h"
#include "..\Headers\Particle.h"
#include "MainCamera.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr), m_eType(EFFECT::End)
	, m_bFrameLoopCheck(false)
{
}

HRESULT CParticle::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (sizeof(EffectBasicArgument) == *static_cast<_uint*>(pArg)) {
			EffectBasicArgument* pArgument = static_cast<EffectBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_eType = pArgument->eType;
			// 동적 생성된 거임
			delete pArg;
		}
	}

	m_pTransformCom->m_TransformDesc.vScale = { 0.5f, 0.5f, 0.5f };

	if (FAILED(CParticle::AddComponents()))
		return E_FAIL;

	return S_OK;
}

_uint CParticle::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	if (m_bFrameLoopCheck) {
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CParticle::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);
	IsBillboarding();

	return _uint();
}

HRESULT CParticle::RenderGameObject()
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

HRESULT CParticle::AddComponents()
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
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Item;
	_Info.bWallCollision = false;
	_Info.bFloorCollision = false;
	_Info.bMapBlock = false;

	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

#pragma region Particle_Texture
	// 아이템 타입으로 텍스처 컴포넌트 추가
	switch (m_eType)
	{
		// 큰 출혈
	case EFFECT::BloodHit_Big:
		m_fFrameCnt = 0;
		m_fStartFrame = 0;
		m_fEndFrame = 8;
		if (FAILED(CGameObject::AddComponent(
			(_int)ESceneID::Static,
			L"Component_Texture_BloodHit_1_Big",
			L"Com_Texture_BloodHit_1_Big",
			(CComponent**)&m_pTexture)))
			return E_FAIL;
		break;
	default:
		// 예약되지 않은 타입
		return E_FAIL;
		break;
	}
#pragma endregion	// Particle_Texture

	return S_OK;
}

// 텍스처 프레임 이동
bool CParticle::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
		return true;
	}
	return false;
}

HRESULT CParticle::IsBillboarding()
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

CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CParticle* pInstance = new CParticle(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CParticle");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle::Clone(void * pArg)
{
	CParticle* pClone = new CParticle(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CParticle");
		SafeRelease(pClone);
	}

	return pClone;
}

void CParticle::Free()
{
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}


