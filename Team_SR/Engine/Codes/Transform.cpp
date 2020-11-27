#include "..\Headers\Transform.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
	ZeroMemory(&m_TransformDesc, sizeof(TRANSFORM_DESC));
	D3DXMatrixIdentity(&m_TransformDesc.matWorld);
}

CTransform::CTransform(const CTransform & other)
	: CComponent(other)
	, m_TransformDesc(other.m_TransformDesc)
{
}

const TRANSFORM_DESC & CTransform::GetTransformDesc() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_TransformDesc;
}

HRESULT CTransform::ReadyComponentPrototype()
{
	return S_OK;
}

HRESULT CTransform::ReadyComponent(void* pArg/* = nullptr*/)
{
	if (nullptr != pArg)
	{
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORM_DESC));
	}

	return S_OK;
}

HRESULT CTransform::UpdateTransform()
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_TransformDesc.vScale.x, m_TransformDesc.vScale.y, m_TransformDesc.vScale.z);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_TransformDesc.vRotation.x));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_TransformDesc.vRotation.y));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_TransformDesc.vRotation.z));
	D3DXMatrixTranslation(&matTrans, m_TransformDesc.vPosition.x, m_TransformDesc.vPosition.y, m_TransformDesc.vPosition.z);

	m_TransformDesc.matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

	return S_OK;
}

void CTransform::GoStraight(float fDeltaTime)
{
	_vector vLook;
	memcpy(&vLook, &m_TransformDesc.matWorld.m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vLook, &vLook);

	m_TransformDesc.vPosition += vLook * m_TransformDesc.fSpeedPerSec * fDeltaTime;
}

void CTransform::GoSide(float fDeltaTime)
{
	_vector vRight;
	memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(_vector));
	D3DXVec3Normalize(&vRight, &vRight);

	m_TransformDesc.vPosition += vRight * m_TransformDesc.fSpeedPerSec * fDeltaTime;
}

void CTransform::AddRotationX(float fDelaTime)
{
	m_TransformDesc.vRotation.x += m_TransformDesc.fRotatePerSec * fDelaTime;
}

void CTransform::AddRotationY(float fDelaTime)
{
	m_TransformDesc.vRotation.y += m_TransformDesc.fRotatePerSec * fDelaTime;
}

void CTransform::AddRotationZ(float fDelaTime)
{
	m_TransformDesc.vRotation.z += m_TransformDesc.fRotatePerSec * fDelaTime;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform* pInstance = new CTransform(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Transform");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg/* = nullptr*/)
{
	CTransform* pClone = new CTransform(*this);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone Transform");
		SafeRelease(pClone);
	}

	return pClone;
}

void CTransform::Free()
{
	CComponent::Free();
}
