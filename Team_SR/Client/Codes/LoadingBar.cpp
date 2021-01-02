#include "stdafx.h"
#include "..\Headers\LoadingBar.h"
#include "ImGuiHelper.h"
#include "VIBuffer_UITexture.h"
#include "UIManager.h"

using namespace UI_AddTag;
CLoadingBar::CLoadingBar(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_fMaxSize(0.f)
{

}


HRESULT CLoadingBar::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBar::ReadyGameObject(void * pArg/* = nullptr*/)
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
	m_fMaxSize = m_UIDesc.vUISize.x;
	m_UIDesc.vUIPos = tagInput.tUIDesc.vUIPos;
	m_UIDesc.vCenter = tagInput.tUIDesc.vCenter;
	m_bTextOut = tagInput.bTextOut;

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		PRINT_LOG(L"Warning", L"Sprite Createing Failed! - CGraphic_Device.cpp");
		return E_FAIL;
	}

	//Font
	if (FAILED(D3DXCreateFont(m_pDevice, 20, 20, 1000, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"신명조", &m_pFont)))
	{
		PRINT_LOG(L"Warning", L"폰트불러오기 실패");
		return E_FAIL;
	}

	return S_OK;
}

_uint CLoadingBar::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);

	if (!m_bShown || !m_piMaxValue || !m_piMinValue)
		return S_OK;

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

	//ImGui::Separator();
	//ImGui::SliderFloat("Ratio Percent",
	//	reinterpret_cast<float*>(&m_fRatio),
	//	0.f, +1.f, "%f");

	

	ImGui::Separator();
	ImGui::SliderInt(" Max",
		reinterpret_cast<int*>(m_piMaxValue),
		0 , 1000, "%d");
	ImGui::Separator();
	ImGui::SliderInt(" Min",
		reinterpret_cast<int*>(m_piMinValue),
		0, 1000, "%d");
	ImGui::End();

	
	m_UIDesc.vUISize.x = m_fMaxSize * m_fRatio;


	return _uint();
}

_uint CLoadingBar::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown || !m_piMaxValue || !m_piMinValue)
		return S_OK;
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (m_piMinValue && m_piMaxValue)
	{
		m_fRatio = (float)*m_piMinValue / (float)*m_piMaxValue;
		if (1.f < m_fRatio)
			m_fRatio = 1.f;
	}

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CLoadingBar::RenderGameObject()
{
	if (!m_bShown || !m_piMaxValue || !m_piMinValue)
		return S_OK;

	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_UIDesc.matWorld)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_UIDesc.matView)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_UIDesc.matOrthographic)))
		return E_FAIL;

	if (0.f > m_UIDesc.vCenter.x)
		static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->SetDisUVposX(m_fRatio);
	else
		static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->ReverseSetDisUVposX(m_fRatio);

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->ResetDisUVpos();

	if (m_bTextOut)
		RenderText();

	return S_OK;
}

void CLoadingBar::SetMaxValueAndMinValue(_int* _piMaxValue, _int* _piValue)
{
	m_piMaxValue = _piMaxValue;
	m_piMinValue = _piValue;
}

int CLoadingBar::GetMaxValue()
{
	if (!m_piMaxValue)
		return 0;
	return *m_piMaxValue;
}

int CLoadingBar::GetMinValue()
{
	if (!m_piMinValue)
		return 0;
	return *m_piMinValue;
}


HRESULT CLoadingBar::AddComponent(wstring _PrototypeTag, wstring _ComponentTag)
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

HRESULT CLoadingBar::RenderText()
{
	TCHAR szRenderLife[64] = L"";
	swprintf_s(szRenderLife, L"%d / %d", *m_piMinValue, *m_piMaxValue);

	_vector vTextSize = { 2.f, 1.f, 0.f };
	_vector vConvertUIPos = m_UIDesc.vUIPos;
	vConvertUIPos.y = -m_UIDesc.vUIPos.y;
	vConvertUIPos.x += (float)(WINCX / 2);
	vConvertUIPos.y += (float)(WINCY / 2);

	if (0.f <= m_UIDesc.vCenter.x)
	{
		vConvertUIPos.x -= m_UIDesc.vCenter.x * m_fMaxSize;
	}

	//y축은 위로 올림
	if (0.f <= m_UIDesc.vCenter.y)
	{
		vConvertUIPos.y -= (m_UIDesc.vUISize.y / 2.f) - 10.f;
	}

	RECT rc = { vConvertUIPos.x - (vTextSize.x / 2), vConvertUIPos.y - (vTextSize.y / 2),
		vConvertUIPos.x + (vTextSize.x / 2), vConvertUIPos.y + (vTextSize.y / 2) };
	m_pFont->DrawText(NULL, szRenderLife, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	return S_OK;
}


CLoadingBar * CLoadingBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CLoadingBar* pInstance = new CLoadingBar(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CLoadingBar");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CLoadingBar::Clone(void * pArg/* = nullptr*/)
{
	CLoadingBar* pClone = new CLoadingBar(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CLoadingBar");
		SafeRelease(pClone);
	}

	return pClone;
}


void CLoadingBar::Free()
{

	//m_pFont->Release();
	CGameUI::Free();
}

