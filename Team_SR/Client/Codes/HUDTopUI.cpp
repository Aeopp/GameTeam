#include "stdafx.h"
#include "..\Headers\HUDTopUI.h"
#include "ImGuiHelper.h"
#include "Loading.h"
#include "UIManager.h"
#include "LoadingBar.h"

using namespace UI_AddTag;
CHUDTopUI::CHUDTopUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice)
{
}

HRESULT CHUDTopUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
#pragma region Add_Prototype
#pragma region Component_Texture_HUDBossUI
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_HUDBossUI",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_monster_HP.png", 1))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_HUDBossBar
	if (FAILED(pManagement->AddComponentPrototype(
		(_int)ESceneID::Static,
		L"Component_Texture_HUDBossHPBar",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bar_monster_HP.png", 1))))
		return E_FAIL;
	
#pragma endregion
#pragma endregion

	return S_OK;
}

HRESULT CHUDTopUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;
	else
		m_pMonsterHPBar = static_cast<CLoadingBar*>(pArg);

	if (FAILED(AddComponent()))
		return E_FAIL;

	//UI Top
	m_UIDesc.vUISize.x = WINCX;
	m_UIDesc.vUISize.y = 100;
	m_UIDesc.vUISize.z = 0.f;

	m_UIDesc.vUIPos.x = 0.f;
	m_UIDesc.vUIPos.y = (WINCY / 2) - (m_UIDesc.vUISize.y / 2.f);
	m_UIDesc.vUIPos.z = 3.f;

	m_UIDesc.vCenter = _vector(0.f, 0.f, 0.f);

	//UI BosHPBar
	m_tBossUIDesc.vUISize.x = WINCX - 500.f;
	//m_tBossUIDesc.vUISize.x = 0.f;
	m_tBossUIDesc.vUISize.y = 60.f;
	m_tBossUIDesc.vUISize.z = 0.f;
	
	m_tBossUIDesc.vUIPos.x = 0.f;
	m_tBossUIDesc.vUIPos.y = (WINCY / 2) - (m_UIDesc.vUISize.y);
	m_tBossUIDesc.vUIPos.z = 3.f;
	m_tBossUIDesc.vCenter = _vector(0.f, 0.f, 0.f);

	m_pMonsterHPBar->SetInvisibleUI();

	return S_OK;
}

_uint CHUDTopUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);

	//ImGui::Begin("HUD Top UI Edit");
	////UI Top
	//ImGui::Separator();
	//ImGui::SliderFloat3("HUD Top UI Size",
	//	reinterpret_cast<float*>(&m_UIDesc.vUISize),
	//	-1000.f, +1000.f, "%f");

	//ImGui::Separator();
	//ImGui::SliderFloat3("HUD Top UI Location",
	//	reinterpret_cast<float*>(&m_UIDesc.vUIPos),
	//	-1000.f, +2000.f, "%f");

	////UI BosHPBar
	//ImGui::Separator();
	//ImGui::SliderFloat3("BossUIDesc Size",
	//	reinterpret_cast<float*>(&m_tBossUIDesc.vUISize),
	//	-1000.f, +1000.f, "%f");

	//ImGui::Separator();
	//ImGui::SliderFloat3("BossUIDesc Location",
	//	reinterpret_cast<float*>(&m_tBossUIDesc.vUIPos),
	//	-1000.f, +2000.f, "%f");
	//ImGui::Separator();
	//
	//ImGui::SliderInt("TopUIDesc Shown",
	//	reinterpret_cast<int*>(&m_bShown),
	//	0, 1, "%f");
	//ImGui::SliderInt("BossUIDesc Shown",
	//	reinterpret_cast<int*>(&m_bMonsterHPbarShown),
	//	0, 1, "%f");

	//ImGui::End();

	//���� HP���� ������Ʈ
	if (!m_pMonsterHPBar)
		return _uint();
	else
	{
		//Monster is Dead!!
		//Reset input the SetMaxHPAndHP()
		if (0 >= m_pMonsterHPBar->GetFloatMinValue())
		{
			m_bMonsterHPbarShown = false;
			m_pMonsterHPBar->SetInvisibleUI();
		}
	}

	return _uint();
}

_uint CHUDTopUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;

	CGameUI::LateUpdateGameObject(fDeltaTime);

	CGameUI::SetupUIMatrix(m_tBossUIDesc);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this, ERenderPlace::BACK)))
		return 0;

	return _uint();
}

HRESULT CHUDTopUI::RenderGameObject()
{
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

	if (!m_bMonsterHPbarShown)
	{
		m_pMonsterHPBar->SetInvisibleUI();
		return S_OK;
	}
	else
	{
		m_pMonsterHPBar->SetShownUI();
	}
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_tBossUIDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_tBossUIDesc.matView)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_tBossUIDesc.matOrthographic)))
		return E_FAIL;

	if (FAILED(m_pBossHPTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pBossHPVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHUDTopUI::AddComponent()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(	/*�̰� ���ο� UI���۸� ���Ѱ�*/
		(_int)ESceneID::Static,
		L"Component_VIBuffer_UITexture",
		L"Com_BossHPVIBuffer",
		(CComponent**)&m_pBossHPVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(CGameObject::AddComponent(/*�̰� ���� UI�ؽ��ĸ� ���Ѱ�*/
		(_int)ESceneID::Static,
		L"Component_Texture_HUDTopUI",
		L"Com_Texture_HUDTopUI",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(/*�̰� ���ο� UI�ؽ��ĸ� ���Ѱ�*/
		(_int)ESceneID::Static,
		L"Component_Texture_HUDBossUI",
		L"Com_Texture_HUDBossUI",
		(CComponent**)&m_pBossHPTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

void CHUDTopUI::SetMaxHPAndHP(float* _pfMaxValue, float* _pfValue)
{
	m_bMonsterHPbarShown = true;
	m_pMonsterHPBar->SetMaxValueAndMinValue(_pfMaxValue, _pfValue);
	m_pMonsterHPBar->SetShownUI();
}

CHUDTopUI * CHUDTopUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CHUDTopUI* pInstance = new CHUDTopUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CLoadingBar");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CHUDTopUI::Clone(void * pArg/* = nullptr*/)
{
	CHUDTopUI* pClone = new CHUDTopUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CHUDTopUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CHUDTopUI::Free()
{
	CGameUI::Free();
}
