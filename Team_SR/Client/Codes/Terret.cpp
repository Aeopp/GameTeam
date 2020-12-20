#include "stdafx.h"
#include "..\Headers\Terret.h"

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
	return S_OK;
}

_uint CTerret::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);
	return _uint();
}

_uint CTerret::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);
	return _uint();
}

HRESULT CTerret::RenderGameObject()
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

HRESULT CTerret::AddComponents()
{
	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		CComponent::Tag + TYPE_NAME<CVIBuffer_RectTexture>(),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

#pragma region Add_Component_Texture
	wstring wstrTexture = CComponent::Tag + TYPE_NAME<CTexture>();
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		wstrTexture + L"Terret",
		L"Com_Texture_Terret",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	return S_OK;
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
