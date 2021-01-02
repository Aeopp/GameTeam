#include "stdafx.h"
#include "..\Headers\GlacierParticle.h"
#include "NormalUVVertexBuffer.h"

CGlacierParticle::CGlacierParticle(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
{
}

HRESULT CGlacierParticle::ReadyGameObjectPrototype()
{
	if (FAILED(CEffect::ReadyGameObjectPrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlacierParticle::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CEffect::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
	}

	//m_pTransformCom->m_TransformDesc.vPosition = { 10.f,10.f,20.f };
	m_fStartY = m_pTransformCom->m_TransformDesc.vPosition.y;
	m_fJumpPower = 15.f;
	m_pTransformCom->m_TransformDesc.vScale = { 0.5f,0.5f,0.5f };
	int iRandomX = rand() % 10 - 5;
	int iRandomZ = rand() % 10 - 5;

	m_vLook = { (float)iRandomX,0.f,(float)iRandomZ };
	m_fFrameCnt = rand() % 5;


	return S_OK;
}

_uint CGlacierParticle::UpdateGameObject(float fDeltaTime)
{
	if (m_bStop)
		return 0;
	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime;
	m_pTransformCom->m_TransformDesc.vPosition.y = m_fStartY + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime);
	m_fJumpTime += 0.03f;
	CEffect::UpdateGameObject(fDeltaTime);

	//_CollisionComp->bMapCollision = true;
	_CollisionComp->Update(m_pTransformCom);
	return _uint();
}

_uint CGlacierParticle::LateUpdateGameObject(float fDeltaTime)
{
	CEffect::LateUpdateGameObject(fDeltaTime);

	CEffect::IsBillboarding();

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;



	return _uint();
}

HRESULT CGlacierParticle::RenderGameObject()
{
	if (FAILED(CEffect::RenderGameObject()))
		return E_FAIL;

	const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			IDirect3DBaseTexture9* const  DiffuseTexture = m_pTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
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
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

void CGlacierParticle::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	m_bStop = true;
	m_pTransformCom->m_TransformDesc.vPosition.y = _CollisionInfo.IntersectPoint.y;
}

HRESULT CGlacierParticle::AddComponents()
{
	if (FAILED(CEffect::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"GlacierParticle",
		L"Com_Texture_GlacierParticle",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 0.75f;
	_Info.Tag = CCollisionComponent::ETag::Particle;
	_Info.bFloorCollision = true;
	_Info.bWallCollision = false;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	return S_OK;
}

CGlacierParticle * CGlacierParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacierParticle* pInstance = new CGlacierParticle(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacierParticle");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CGlacierParticle::Clone(void * pArg /*= nullptr*/)
{
	CGlacierParticle* pClone = new CGlacierParticle(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacierParticle");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacierParticle::Free()
{
	SafeRelease(_VertexBuffer);
	CEffect::Free();
}
