#include "stdafx.h"
#include "..\Headers\LoadingBar.h"
#include "ImGuiHelper.h"
#include "VIBuffer_UITexture.h"
#include "UIManager.h"


CLoadingBar::CLoadingBar(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_fMaxSize(312.f)
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

	ZeroMemory(&m_tFontInfo, sizeof(D3DXFONT_DESCW));

	m_tFontInfo.Height = 20;
	m_tFontInfo.Width = 10;
	m_tFontInfo.Weight = FW_HEAVY;
	m_tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(m_tFontInfo.FaceName, L"돋움체");

		UI_ADD_COMPONENT tagInput = *static_cast<UI_ADD_COMPONENT*>(pArg);;

	if (FAILED(AddComponent(tagInput.wsPrototypeTag, tagInput.wsComponentTag)))
		return E_FAIL;

	m_wsObjectName = tagInput.wsPrototypeTag;
	m_UIDesc.vUISize = tagInput.tUIDesc.vUISize;
	m_fMaxSize = m_UIDesc.vUISize.x;
	m_UIDesc.vUIPos = tagInput.tUIDesc.vUIPos;
	m_UIDesc.vCenter = tagInput.tUIDesc.vCenter;


	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		PRINT_LOG(L"Warning", L"Sprite Createing Failed! - CGraphic_Device.cpp");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &m_tFontInfo, &m_pFont)))
	{
		PRINT_LOG(L"Warning", L"폰트불러오기 실패");
		return E_FAIL;
	}

	return S_OK;
}

_uint CLoadingBar::UpdateGameObject(float fDeltaTime)
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
	CGameUI::LateUpdateGameObject(fDeltaTime);

	m_fRatio = (float)*m_piMinValue / (float)*m_piMaxValue;
	if (1.f < m_fRatio)
		m_fRatio = 1.f;

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CLoadingBar::RenderGameObject()
{
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

	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	static_cast<CVIBuffer_UITexture*>(m_pVIBufferCom)->ResetDisUVpos();

	TCHAR szRenderLife[64] = L"";
	swprintf_s(szRenderLife, L"%d / %d", *m_piMinValue, *m_piMaxValue);

	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, m_UIDesc.vUIPos.x, m_UIDesc.vUIPos.y, m_UIDesc.vUIPos.z);
	matWorld = matScale * matTrans;

	m_pSprite->SetTransform(&matWorld);
	//m_pFont->DrawTextW(m_pSprite, szRenderLife, lstrlen(szRenderLife),
	//	nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pFont->DrawTextW(m_pSprite, szRenderLife, lstrlen(szRenderLife),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	
	return S_OK;
}

void CLoadingBar::SetMaxHPAndHP(int* _piMaxValue, int* _piValue)
{
	m_piMaxValue = _piMaxValue;
	m_piMinValue = _piValue;
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
