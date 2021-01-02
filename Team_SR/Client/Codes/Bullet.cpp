#include "stdafx.h"
#include "..\Headers\Bullet.h"
#include "Camera.h"
#include "NormalUVVertexBuffer.h"
CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_pTexture(nullptr)
{
}

HRESULT CBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		// 2020.12.16 17:35 KMJ
		// 아규먼트 메모리 해제
		// 구조체 크기 검사
		if (sizeof(BulletBasicArgument) == *static_cast<_uint*>(pArg)) {
			BulletBasicArgument* pArgument = static_cast<BulletBasicArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_vLook = pArgument->vDir;
			delete pArg;
		}
	}

	return S_OK;
}

_uint CBullet::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CBullet::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)-1, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);

	return _uint();
}

HRESULT CBullet::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
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

HRESULT CBullet::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	return S_OK;
}

// 텍스처 프레임 이동
void CBullet::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += 10.f * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

void CBullet::Free()
{
	SafeRelease(_VertexBuffer);
	SafeRelease(m_pVIBufferCom);	// 버텍스 버퍼
	SafeRelease(m_pTexture);		// 텍스처

	CGameObject::Free();
}


