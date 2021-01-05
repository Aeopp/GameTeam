#include "stdafx.h"
#include "FaceUI.h"
#include "ImGuiHelper.h"
#include "UIManager.h"
#include "LoadingBar.h"


CFaceUI::CFaceUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_eFaceState(EFaceState::UNHURT), m_fAniTime(0.f)
{
}

HRESULT CFaceUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFaceUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;
	AddComponent();

	m_UIDesc.vUISize.x = 170;
	m_UIDesc.vUISize.y = 170;
	m_UIDesc.vUISize.z = 0.f;

	m_UIDesc.vUIPos.x = -840.f;
	m_UIDesc.vUIPos.y = -420.f;
	m_UIDesc.vUIPos.z = 0.f;
	m_UIDesc.vCenter = _vector(0.f, 0.f, 0.f);
	return S_OK;
}

_uint CFaceUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);

	ImGui::Begin("Face UI");

	ImGui::Separator();
	ImGui::SliderFloat3("Size",
		reinterpret_cast<float*>(&m_UIDesc.vUISize),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_UIDesc.vUIPos),
		-1000.f, +1000.f, "%f");

	ImGui::End();

	return _uint();
}

_uint CFaceUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	CLoadingBar* m_pHpBar = static_cast<CLoadingBar*>
		(CManagement::Get_Instance()->GetGameObject((_int)CUIManager::Get_Instance()->GetSceneID(),
		L"Layer_HUD_HpBar"));

	int iMax = m_pHpBar->GetMaxValue();
	int iMin = m_pHpBar->GetMinValue();

	if (0.9f <= (float)iMax / (float)iMin)
		m_eFaceState = EFaceState::UNHURT;
	else if (0.7f <= (float)iMax / (float)iMin)
		m_eFaceState = EFaceState::BLEEDING;
	else if (0.3f <= (float)iMax / (float)iMin)
		m_eFaceState = EFaceState::INJURED;
	else if (0 == iMin)
		m_eFaceState = EFaceState::DYING;

	m_fAniTime += fDeltaTime;

	if ((float)3 <= m_fAniTime)
		m_fAniTime = 0.f;

	return _uint();
}

HRESULT CFaceUI::RenderGameObject()
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

	CTexture* pTexture = nullptr;

	switch (m_eFaceState)
	{
	case CFaceUI::EFaceState::UNHURT:
		pTexture = m_pTextureCom;
		break;
	case CFaceUI::EFaceState::BLEEDING:
		pTexture = m_pBleedingTexCom;
		break;
	case CFaceUI::EFaceState::INJURED:
		pTexture = m_pInjuredTexCom;
		break;
	case CFaceUI::EFaceState::DYING:
		pTexture = m_pDyingTexCom;
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(pTexture->Set_Texture((_uint)m_fAniTime)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFaceUI::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ani_Face_Unhurt",
		L"Com_Texture_Unhurt",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ani_Face_Bleeding",
		L"Com_Texture_Bleeding",
		(CComponent**)&m_pBleedingTexCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ani_Face_Injured",
		L"Com_Texture_Injured",
		(CComponent**)&m_pInjuredTexCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Ani_Face_Dying",
		L"Com_Texture_Dying",
		(CComponent**)&m_pDyingTexCom)))
		return E_FAIL;

	return S_OK;
}

CFaceUI * CFaceUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CFaceUI* pInstance = new CFaceUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CFaceUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CFaceUI::Clone(void * pArg/* = nullptr*/)
{
	CFaceUI* pClone = new CFaceUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CFaceUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CFaceUI::Free()
{
	CGameUI::Free();
}
