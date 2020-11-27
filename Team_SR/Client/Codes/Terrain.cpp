#include "stdafx.h"
#include "..\Headers\Terrain.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CTerrain::CTerrain(const CTerrain & other)
	: CGameObject(other)	
{
}

HRESULT CTerrain::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::ReadyGameObject(void * pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	return S_OK;
}

_uint CTerrain::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);	

	//m_pTransformCom->UpdateTransform();

	return _uint();
}

_uint CTerrain::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CTerrain::RenderGameObject()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->GetTransformDesc().matWorld)))
		return E_FAIL;

	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Stage,
		L"Component_Texture_Terrain",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Stage,
		L"Component_VIBuffer_TerrainTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Transform",
		L"Com_Transform",
		(CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CTerrain* pInstance = new CTerrain(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CTerrain");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void * pArg)
{
	CTerrain* pClone = new CTerrain(*this); /* 복사생성자 */
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CTerrain::Free()
{
	SafeRelease(m_pVIBufferCom);
	SafeRelease(m_pTransformCom);
	SafeRelease(m_pTextureCom);

	CGameObject::Free();
}
