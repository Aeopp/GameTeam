#include "stdafx.h"
#include "..\Headers\GlacierBullet.h"
#include "DXWrapper.h"



CGlacierBullet::CGlacierBullet(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CGlacierBullet::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlacierBullet::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (sizeof(BulletBasicArgument) == *(_uint*)pArg)
	{
		BulletBasicArgument* pData = (BulletBasicArgument*)pArg;
		m_vLook  = pData->vDir;
		m_vLook.y = 0.f;
		m_pTransformCom->m_TransformDesc.vPosition = pData->vPosition;
		delete pData;
		pData = nullptr;
	}

	//m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 500.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fSpeed = 5.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 3;
	return S_OK;
}

_uint CGlacierBullet::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	
	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag ^= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	//_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CGlacierBullet::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CGlacierBullet::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	//const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	//auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	//// 현재 사용중이던 텍스쳐를 여기에 세팅.
	//{
	//	//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
	//	{
	//		IDirect3DBaseTexture9* const  DiffuseTexture = m_pTexture->GetTexture((_uint)m_fFrameCnt);

	//		m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
	//	}
	//	// 1.       그냥 세팅을 안하거나
	//	{
	//		_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
	//		_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
	//	}
	//	// 2. 세팅을 하고 난 이후의                                   ↑↑↑↑↑↑↑↑↑↑     TRUE 로 바꾸어주기.
	//	{
	//		// m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),SpecularTexture);
	//		// m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),NormalTexture);
	//	}
	//}
	//// 월드 행렬 바인딩
	//_Effect.SetVSConstantData(m_pDevice, "World", World);
	//// 광택 설정 
	//_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	//m_pDevice->SetVertexShader(_Effect.VsShader);
	//m_pDevice->SetPixelShader(_Effect.PsShader);
	//_VertexBuffer->Render();

	////if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	////	return E_FAIL;

	////if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
	////	return E_FAIL;

	////if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	////	return E_FAIL;
	return S_OK;
}

HRESULT CGlacierBullet::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	// 글레이서 불렛 텍스처
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"GlacierBullet",
		L"Com_Texture_GlacierBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion



	 //충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = false;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bWallCollision = false;
	_Info.bFloorCollision = false;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);
	return S_OK;
}

CGlacierBullet * CGlacierBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacierBullet* pInstance = new CGlacierBullet(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacierBullet");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CGlacierBullet::Clone(void * pArg /*= nullptr*/)
{
	CGlacierBullet* pClone = new CGlacierBullet(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacierBullet");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacierBullet::Free()
{

	CBullet::Free();
}
