#include "stdafx.h"
#include "..\Headers\HellBossEyeBlast.h"


CHellBossEyeBlast::CHellBossEyeBlast(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CHellBossEyeBlast::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellBossEyeBlast::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,3.f,2.5f };

	// 불렛 원본 스텟
	m_stOriginStatus.dwPiercing = 0;
	m_stOriginStatus.fRange = 300.f;
	m_stOriginStatus.fATK = 10.f;
	m_stOriginStatus.fSpeed = 60.f;
	m_stOriginStatus.fImpact = 0.f;
	// 인게임에서 사용할 스텟
	m_stStatus = m_stOriginStatus;

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 9;

	// 로컬에서 회전
	const float AngleX = 90.f;
	const float AngleY = 90.f;
	mat RotX, RotY;
	D3DXMatrixRotationX(&RotX, MATH::ToRadian(AngleX));
	D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));

	vec3 CameraLook = { 0,0,-1 };
	vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, m_vLook));
	float Angle = std::acosf(MATH::Dot(m_vLook, CameraLook));
	mat RotAxis;	// 쏘려는 각도 행렬
	D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
	m_matRot = RotX * RotY * RotAxis;	// 원본 각도 * 쏘려는 각도

	return S_OK;
}

_uint CHellBossEyeBlast::UpdateGameObject(float fDeltaTime)
{
	//CBullet::UpdateGameObject(fDeltaTime);	// 기본 게임오브젝트 업데이트 X

	// 스케일
	m_pTransformCom->m_TransformDesc.vScale.x += (m_stStatus.fSpeed * fDeltaTime * 2.f);

	// 월드 행렬 셋팅
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_pTransformCom->m_TransformDesc.vScale.x, m_pTransformCom->m_TransformDesc.vScale.y, m_pTransformCom->m_TransformDesc.vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pTransformCom->m_TransformDesc.vPosition.x, m_pTransformCom->m_TransformDesc.vPosition.y, m_pTransformCom->m_TransformDesc.vPosition.z);
	m_pTransformCom->m_TransformDesc.matWorld = matScale * m_matRot * matTrans;

	vec3 vMoveDstnc = m_vLook * fDeltaTime * m_stStatus.fSpeed;
	m_pTransformCom->m_TransformDesc.vPosition += vMoveDstnc;	// 이동
	m_stStatus.fRange -= D3DXVec3Length(&vMoveDstnc);			// 사거리 차감
	if (m_stStatus.fRange <= 0) {	// 사거리를 전부 차감했으면
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
	}

	_CollisionComp->Update(m_pTransformCom);
	// 플레이어 충돌 처리
	if (Bullet_Attack(m_stStatus.fATK)) {
		m_byObjFlag |= static_cast<BYTE>(ObjFlag::Remove);	// 오브젝트 삭제 플래그 ON
		// 총알 생성
		BulletBasicArgument* pArg = new BulletBasicArgument;
		pArg->uiSize = sizeof(BulletBasicArgument);
		pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition + m_vLook * 6.f;	// 생성 위치
		pArg->vPosition.y = 5.f;
		m_pManagement->AddScheduledGameObjectInLayer(
			(_int)ESceneID::Static,
			L"GameObject_Explosion",
			L"Layer_Bullet",
			nullptr, (void*)pArg);
	}

	return _uint();
}

_uint CHellBossEyeBlast::LateUpdateGameObject(float fDeltaTime)
{
	//CBullet::LateUpdateGameObject(fDeltaTime);	// 빌보드 X

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CHellBossEyeBlast::RenderGameObject()
{
	// 뒷면을 컬링하지 않습니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	// 시계 반대 방향을 컬링합니다
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CHellBossEyeBlast::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
	CBullet::MapHit(_PlaneInfo, _CollisionInfo);

	// 총알 생성
	BulletBasicArgument* pArg = new BulletBasicArgument;
	pArg->uiSize = sizeof(BulletBasicArgument);
	pArg->vPosition = m_pTransformCom->m_TransformDesc.vPosition + m_vLook * 6.f;	// 생성 위치
	pArg->vPosition.y = 5.f;
	m_pManagement->AddScheduledGameObjectInLayer(
		(_int)ESceneID::Static,
		L"GameObject_Explosion",
		L"Layer_Bullet",
		nullptr, (void*)pArg);
}

HRESULT CHellBossEyeBlast::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 레이저 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_ElectricHeavy",
		L"Com_Texture_ElectricHeavy",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.5f;
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

CHellBossEyeBlast* CHellBossEyeBlast::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHellBossEyeBlast* pInstance = new CHellBossEyeBlast(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CHellBossEyeBlast");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CHellBossEyeBlast::Clone(void* pArg/* = nullptr*/)
{
	CHellBossEyeBlast* pClone = new CHellBossEyeBlast(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHellBossEyeBlast");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHellBossEyeBlast::Free()
{
	CBullet::Free();
}
