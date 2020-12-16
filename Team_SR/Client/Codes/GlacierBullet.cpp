#include "stdafx.h"
#include "..\Headers\GlacierBullet.h"


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

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = ((_vector*)pArg)[1];
		m_vLook = ((_vector*)pArg)[0] - m_pTransformCom->m_TransformDesc.vPosition;
	}

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 몬스터 원본 스텟
	m_stOriginStatus.dwRange = 100.f;
	m_stOriginStatus.dwPiercing = 0.f;
	m_stOriginStatus.fATK = 7.f;
	m_stOriginStatus.fSpeed = 15.f;
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

	m_pTransformCom->m_TransformDesc.vPosition += m_vLook * fDeltaTime * m_stStatus.fSpeed;
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

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_Texture((_uint)m_fFrameCnt)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	return S_OK;
}

HRESULT CGlacierBullet::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))
		return E_FAIL;

#pragma region Add_Component_Texture
	// 박쥐 침 텍스처
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"GlacierBullet",
		L"Com_Texture_GlacierBullet",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion
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
