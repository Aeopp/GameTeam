#include "stdafx.h"
#include "..\Headers\WeaponUI.h"
#include "ImGuiHelper.h"
#include "UIManager.h"

USING(UI_AddTag)
_uint CWeaponUI::s_iCloneCount = 0;
LPD3DXFONT CWeaponUI::s_pFont = nullptr;
CWeaponUI::CWeaponUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_cfMaxShownTime(5.f)
{
}

HRESULT CWeaponUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	//Font
	if (FAILED(D3DXCreateFont(m_pDevice, 25, 25, 1000, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"신명조", &s_pFont)))
	{
		PRINT_LOG(L"Warning", L"폰트불러오기 실패");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWeaponUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	UI_BAR_ADD_COMPONENT tagInput = *static_cast<UI_BAR_ADD_COMPONENT*>(pArg);;

	if (FAILED(AddComponent(tagInput.wsPrototypeTag, tagInput.wsComponentTag)))
		return E_FAIL;

	m_wsObjectName = tagInput.wsPrototypeTag;
	m_UIDesc.vUISize = tagInput.tUIDesc.vUISize;
	m_UIDesc.vUIPos = tagInput.tUIDesc.vUIPos;
	m_UIDesc.vCenter = tagInput.tUIDesc.vCenter;

	m_iID = s_iCloneCount++;
	return S_OK;
}

_uint CWeaponUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);

	char* pStr;

	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, m_wsObjectName.c_str(), -1, NULL, 0, NULL, NULL);

	//char* 메모리 할당
	pStr = new char[strSize];

	//형 변환
	WideCharToMultiByte(CP_ACP, 0, m_wsObjectName.c_str(), -1, pStr, strSize, 0, 0);

	ImGui::Begin(pStr);

	delete[] pStr;
	pStr = nullptr;

	ImGui::Separator();
	ImGui::SliderFloat3("Size",
		reinterpret_cast<float*>(&m_UIDesc.vUISize),
		-1000.f, +1000.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_UIDesc.vUIPos),
		-1000.f, +1000.f, "%f");
	
	ImGui::End();

	//m_bShown = true;
	if (!m_bShown)
	{
		m_fShownTime = 0.f;
		return _uint();
	}
	m_fShownTime += fDeltaTime;

	return _uint();
}

_uint CWeaponUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	if (m_cfMaxShownTime <= m_fShownTime)
	{
		m_fShownTime = 0.f;
		m_bShown = false;
		CUIManager::Get_Instance()->AllInvisibleWeaponUI();
	}

	return _uint();
}

HRESULT CWeaponUI::RenderGameObject()
{
	if (!m_bShown)
		return S_OK;

	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

	RenderText();

	if (m_bOnWeapon)
	{
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
	}
	
	return S_OK;
}

HRESULT CWeaponUI::AddComponent(wstring _PrototypeTag, wstring _ComponentTag)
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		_PrototypeTag,
		_ComponentTag,
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponUI::RenderText()
{
	TCHAR szRenderLife[64] = L"";
	swprintf_s(szRenderLife, L"%d", m_iID + 1);

	_vector vTextSize = { 2.f, 1.f, 0.f };
	_vector vConvertUIPos = m_UIDesc.vUIPos;
	vConvertUIPos.y = -m_UIDesc.vUIPos.y;
	vConvertUIPos.x += (float)(WINCX / 2);
	vConvertUIPos.y += (float)(WINCY / 2);

	if (0.f <= m_UIDesc.vCenter.x)
	{
		vConvertUIPos.x -= m_UIDesc.vCenter.x * m_UIDesc.vUISize.x;
	}

	//y축은 위로 올림
	if (0.f <= m_UIDesc.vCenter.y)
	{
		vConvertUIPos.y -= (m_UIDesc.vUISize.y / 2.f) + 10.f;
	}

	RECT rc = { vConvertUIPos.x - (vTextSize.x / 2), vConvertUIPos.y - (vTextSize.y / 2),
		vConvertUIPos.x + (vTextSize.x / 2), vConvertUIPos.y + (vTextSize.y / 2) };
	s_pFont->DrawText(NULL, szRenderLife, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffffff);

	return S_OK;
}

CWeaponUI * CWeaponUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWeaponUI* pInstance = new CWeaponUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CLoadingBar");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CWeaponUI::Clone(void * pArg/* = nullptr*/)
{
	CWeaponUI* pClone = new CWeaponUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CWeaponUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CWeaponUI::Free()
{
	CGameUI::Free();
}
