#include "stdafx.h"
#include "..\Headers\ShopUI.h"
#include "imgui.h"


CShopUI::CShopUI(LPDIRECT3DDEVICE9 pDevice)
	:CGameUI(pDevice)
{
}

HRESULT CShopUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;
	CManagement* pManagement = CManagement::Get_Instance();
#pragma region Component_Texture_HUDBossBar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_ShopUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/Shop/lweaponshop.png", 0))))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

HRESULT CShopUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	m_UIDesc.vUISize.x = 1000.f ;
	m_UIDesc.vUISize.y = 700.f;
	m_UIDesc.vUISize.z = 0;

	m_UIDesc.vUIPos.x = 0.f;
	m_UIDesc.vUIPos.y = 0.f;
	m_UIDesc.vUIPos.z = 2.f;

	m_UIDesc.vCenter = _vector(0.f, 0.f, 0.f);

	return S_OK;
}

_uint CShopUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);
	ImGui::Begin("Shop Edit");
	//UI Top
	ImGui::Separator();
	ImGui::SliderFloat3("Size",
		reinterpret_cast<float*>(&m_UIDesc.vUISize),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_UIDesc.vUIPos),
		-1000.f, +2000.f, "%f");
	return _uint();
}

_uint CShopUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameUI::LateUpdateGameObject(fDeltaTime);

	return _uint();
}

HRESULT CShopUI::RenderGameObject()
{
	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

	if (!m_bShown)
		return S_OK;

	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_UIDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_UIDesc.matView)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_UIDesc.matOrthographic)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShopUI::AddComponent()
{


	return S_OK;
}

CShopUI * CShopUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return nullptr;
}

CGameObject * CShopUI::Clone(void * pArg/* = nullptr*/)
{
	return nullptr;
}

void CShopUI::Free()
{
	CGameUI::Free();
}
