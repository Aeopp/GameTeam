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
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/Shop/lweaponshop.png", 1))))
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

	ImGui::End();
	if (!m_bShown)
		return _uint();


	return _uint();
}

_uint CShopUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return _uint();
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this, ERenderPlace::BACK)))
		return 0;

	return _uint();
}

HRESULT CShopUI::RenderGameObject()
{
	if (!m_bShown)
		return S_OK;

	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

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

void CShopUI::SwitchOnUI()
{
	m_bShown = !m_bShown;
}

void CShopUI::OutButton()
{

}

HRESULT CShopUI::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_ShopUI",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CShopUI * CShopUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CShopUI* pInstance = new CShopUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CShopUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CShopUI::Clone(void * pArg/* = nullptr*/)
{
	CShopUI* pClone = new CShopUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CShopUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CShopUI::Free()
{
	CGameUI::Free();
}
