#include "stdafx.h"
#include "..\Headers\Terret.h"
#include "Camera.h"

CTerret::CTerret(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice),m_fFrameCnt(0.f), m_pTexture(nullptr)
{
}

HRESULT CTerret::ReadyGameObjectPrototype()
{
	CGameObject::ReadyGameObjectPrototype();
	return S_OK;
}

HRESULT CTerret::ReadyGameObject(void * pArg /*= nullptr*/)
{
	CGameObject::ReadyGameObject(pArg);

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vPosition = { 10.f,10.f,20.f };

	return S_OK;
}

_uint CTerret::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	//테스트
	//m_pTransformCom->m_TransformDesc.vRotation.y += 1.f;

	if (m_pTarget)
	{
		if ((BYTE)ObjFlag::Remove & m_pTarget->GetOBjFlag())
		{
			m_pTarget = nullptr;
		}
		Fire(fDeltaTime);
	}
	else
		FindTarget();

	return _uint();
}

_uint CTerret::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);
	if (m_pTarget)
	{
		if ((BYTE)ObjFlag::Remove & m_pTarget->GetOBjFlag())
			m_pTarget = nullptr;
	}
	UpdateAngle();
	//IsBillboarding();

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CTerret::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerret::AddComponents()
{
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTextureTerret = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CTerret>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTextureTerret,
		L"Com_Texture_Terret",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CTerret::FindTarget()
{
	list<CGameObject*> Monsterlist = m_pManagement->GetGameObjects(-1, L"Layer_Monster");
	_vector vDir;
	float	fDistance;
	float	fMin = -1.f;

	const _vector vTerretPosition = m_pTransformCom->m_TransformDesc.vPosition;
	for (auto& pMonster : Monsterlist)
	{
		vDir = vTerretPosition - pMonster->GetTransform()->m_TransformDesc.vPosition;
		fDistance = D3DXVec3Length(&vDir);
		if (fMin < 0.f || fMin > fDistance)
		{
			fMin = fDistance;
			m_pTarget = pMonster;
		}

	}

}

void CTerret::Fire(float fDeltaTime)
{
	if (nullptr == m_pTarget)
		return;
	m_fTestTime += fDeltaTime;
	m_fFrameCnt = 5;
	if (m_fTestTime > 1)
	{
		m_fFrameCnt = 4;
		m_fTestTime = 0.f;
		_vector vDir = m_pTarget->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
		D3DXVec3Normalize(&vDir, &vDir);

		Ray _Ray;
		_Ray.Start = m_pTransformCom->GetLocation();
		//_Ray.Direction = MATH::Normalize(m_pTransformCom->GetLook());
		_Ray.Direction = vDir;

		auto _Component = m_pTarget->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1);

			if (IsCollision.first)
			{
				Collision::Info _CollisionInfo = IsCollision.second;
				m_pTarget->Hit(this, std::move(_CollisionInfo));
			}
		}

	}
}

void CTerret::UpdateAngle()
{
	if (nullptr == m_pTarget)
		return;
	_vector vMyLook = m_pTransformCom->GetLook();
	////_vector vDir = m_pTransformCom->m_TransformDesc.vPosition - m_pTarget->GetTransform()->m_TransformDesc.vPosition;
	_vector vDir = m_pTarget->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	D3DXVec3Normalize(&vDir, &vDir);
	//memcpy(&m_pTransformCom->m_TransformDesc.matWorld[2], &vDir, sizeof(_vector));
	float fDot = D3DXVec3Dot(&vMyLook, &vDir);
	float fAngle = D3DXToDegree(fDot);
	int   iFinalAngle = (_int)fAngle % 360;
	if (iFinalAngle < 0)
	{
		iFinalAngle += 360;
	}
	
	m_pTransformCom->m_TransformDesc.vRotation.y = (float)iFinalAngle;

	//m_fFrameCnt = abs((int)m_fAngle % 360) / 45;
}

void CTerret::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return;

	const auto& _TransformDesc = m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);



}

CTerret * CTerret::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CTerret* pInstance = new CTerret(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CTerret");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CTerret::Clone(void * pArg /*= nullptr*/)
{
	CTerret* pClone = new CTerret(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerret");
		SafeRelease(pClone);
	}

	return pClone;
}

void CTerret::Free()
{
	SafeRelease(m_pVIBufferCom);
	SafeRelease(m_pTexture);
	CGameObject::Free();
}
