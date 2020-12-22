#include "stdafx.h"
#include "..\Headers\HUD_HpBar.h"
#include "ImGuiHelper.h"
#include "VIBuffer_UITexture.h"


CHUD_HpBar::CHUD_HpBar(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_fMaxSize(312.f)
{
}


HRESULT CHUD_HpBar::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHUD_HpBar::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	m_UIDesc.vUISize.x = m_fMaxSize;
	m_UIDesc.vUISize.y = 60.f;
	//m_UIDesc.vUISize.x = WINCX / 10.f;
	//m_UIDesc.vUISize.y = WINCY / 10.f;
	m_UIDesc.vUISize.z = 0;

	//m_UIDesc.vUIPos.x = (WINCX / 2);-754 -403
	m_UIDesc.vUIPos.x = -742;
	m_UIDesc.vUIPos.y = -410;
	m_UIDesc.vUIPos.z = 0.f;

	m_UIDesc.vCenter = _vector(-1.f, 0.f, 0.f);


	return S_OK;
}

_uint CHUD_HpBar::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);
	ImGui::Begin("HUD_BAR Edit");

	ImGui::Separator();
	ImGui::SliderFloat3("Size",
		reinterpret_cast<float*>(&m_UIDesc.vUISize),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_UIDesc.vUIPos),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat("Ratio Percent",
		reinterpret_cast<float*>(&m_fRatio),
		0.f, +1.f, "%f");
	ImGui::End();
	
	m_UIDesc.vUISize.x = m_fMaxSize * m_fRatio;
	

	return _uint();
}

_uint CHUD_HpBar::LateUpdateGameObject(float fDeltaTime)
{
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;



	return _uint();
}

HRESULT CHUD_HpBar::RenderGameObject()
{
	CGameUI::RenderGameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_UIDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_UIDesc.matView)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_UIDesc.matOrthographic)))
		return E_FAIL;

	static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->SetDisUVposX(m_fRatio);
	
	//if (FAILED(CGameObject::RenderGameObject()))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	
	static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->ResetDisUVpos();
	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

void CHUD_HpBar::SetMaxHPAndHP(int& _iMaxHP, int& _iHP)
{
	m_fRatio = _iMaxHP / _iHP;
}

HRESULT CHUD_HpBar::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_HPbar",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}


CHUD_HpBar * CHUD_HpBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHUD_HpBar* pInstance = new CHUD_HpBar(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create HPbar");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CHUD_HpBar::Clone(void * pArg/* = nullptr*/)
{
	CHUD_HpBar* pClone = new CHUD_HpBar(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone HPbar");
		SafeRelease(pClone);
	}

	return pClone;
}


void CHUD_HpBar::Free()
{
	CGameUI::Free();
}
