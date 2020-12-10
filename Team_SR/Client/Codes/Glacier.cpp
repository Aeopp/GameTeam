#include "stdafx.h"
#include "..\Headers\Glacier.h"


CGlacier::CGlacier(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}


HRESULT CGlacier::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlacier::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vPosition = _vector{ -20.f,1.f,30.f };

	return S_OK;
}

_uint CGlacier::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CGlacier::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CGlacier::RenderGameObject()
{

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlacier::AddComponents()
{
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CGlacier>(),
		CComponent::Tag + TYPE_NAME<CTexture>(),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;
	return S_OK;
}

CGlacier* CGlacier::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacier* pInstance = new CGlacier(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacier");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CGlacier::Clone(void* pArg/* = nullptr*/)
{
	CGlacier* pClone = new CGlacier(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacier::Free()
{
	CGameObject::Free();
}
