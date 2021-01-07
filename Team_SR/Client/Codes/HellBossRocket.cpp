#include "stdafx.h"
#include "..\Headers\HellBossRocket.h"
#include "NormalUVVertexBuffer.h"


CHellBossRocket::CHellBossRocket(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CHellBossRocket::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossRocket::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };	// 가로
	m_pTransformVertical = m_pTransformCom->m_TransformDesc;		// 세로
	m_pTransformBack = m_pTransformCom->m_TransformDesc;			// 뒤
	m_pTransformBack.vPosition -= m_vLook;

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 100.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 11;

	// 로컬에서 가로 텍스처 회전
	const float AngleY = 90.f;
	const float AngleZ = 90.f;
	mat RotY, RotZ;
	D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));
	D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(AngleZ));

	vec3 CameraLook = { 0,0,-1 };
	vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, m_vLook));
	float Angle = std::acosf(MATH::Dot(m_vLook, CameraLook));
	mat RotAxis;	// 쏘려는 각도 행렬
	D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
	m_matHorizontalRot = RotY * RotZ * RotAxis;

	// 로컬에서 세로 텍스처 회전
	m_matVerticalRot = RotY * RotAxis;

	// 로컬에서 뒤 텍스처 회전
	m_matBackRot = RotAxis;

	return S_OK;
}

_uint CHellBossRocket::UpdateGameObject(float fDeltaTime)
{
	//CBullet::UpdateGameObject(fDeltaTime);	// 기본 게임오브젝트 업데이트 X

	// 가로 텍스처 월드 행렬 셋팅
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransformCom->m_TransformDesc.vScale.x, m_pTransformCom->m_TransformDesc.vScale.y, m_pTransformCom->m_TransformDesc.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pTransformCom->m_TransformDesc.vPosition.x, m_pTransformCom->m_TransformDesc.vPosition.y, m_pTransformCom->m_TransformDesc.vPosition.z);
	m_pTransformCom->m_TransformDesc.matWorld = matScale * m_matHorizontalRot * matTrans;

	// 세로 텍스처 월드 행렬 셋팅
	D3DXMatrixScaling(&matScale, m_pTransformVertical.vScale.x, m_pTransformVertical.vScale.y, m_pTransformVertical.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pTransformVertical.vPosition.x, m_pTransformVertical.vPosition.y, m_pTransformVertical.vPosition.z);
	m_pTransformVertical.matWorld = matScale * m_matVerticalRot * matTrans;

	// 뒤 텍스처 월드 행렬 셋팅
	D3DXMatrixScaling(&matScale, m_pTransformBack.vScale.x, m_pTransformBack.vScale.y, m_pTransformBack.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pTransformBack.vPosition.x, m_pTransformBack.vPosition.y, m_pTransformBack.vPosition.z);
	m_pTransformBack.matWorld = matScale * m_matBackRot * matTrans;


	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_pTransformVertical.vPosition += vMoveDstnc;
	m_pTransformBack.vPosition += vMoveDstnc;
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);

	return _uint();
}

_uint CHellBossRocket::LateUpdateGameObject(float fDeltaTime)
{
	//CBullet::LateUpdateGameObject(fDeltaTime);	// 빌보드 X

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	//Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossRocket::RenderGameObject()
{
	// 뒷면을 컬링하지 않습니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 부모 랜더를 사용하지 않음
	//if (FAILED(CBullet::RenderGameObject()))
	//	return E_FAIL;

	// 가로 텍스처
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			IDirect3DBaseTexture9* const  DiffuseTexture = m_pHorizontalTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();


	// 세로 텍스처
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformVertical.matWorld)))
		return E_FAIL;

	World = m_pTransformVertical.matWorld;
	_Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			IDirect3DBaseTexture9* const  DiffuseTexture = m_pVerticalTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();


	// 뒤 텍스처
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformBack.matWorld)))
		return E_FAIL;

	World = m_pTransformBack.matWorld;
	_Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			IDirect3DBaseTexture9* const  DiffuseTexture = m_pBackTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();



	// 시계 반대 방향을 컬링합니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CHellBossRocket::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	// 로켓 가로 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_Rocket",
		L"Com_Texture_TurboSatan_Horizontal_Rocket",
		(CComponent**)&m_pHorizontalTexture)))
		return E_FAIL;

	// 로켓 세로 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_Rocket",
		L"Com_Texture_TurboSatan_Vertical_Rocket",
		(CComponent**)&m_pVerticalTexture)))
		return E_FAIL;

	// 로켓 뒤 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HellBoss_TurboSatan_RocketBack",
		L"Com_Texture_TurboSatan_Back_Rocket",
		(CComponent**)&m_pBackTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
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

CHellBossRocket* CHellBossRocket::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossRocket* pInstance = new CHellBossRocket(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossRocket");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossRocket::Clone(void* pArg/* = nullptr*/)
{
	CHellBossRocket* pClone = new CHellBossRocket(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossRocket");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossRocket::Free()
{
	SafeRelease(m_pHorizontalTexture);
	SafeRelease(m_pVerticalTexture);
	SafeRelease(m_pBackTexture);

	CBullet::Free();
}
