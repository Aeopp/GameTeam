#include "stdafx.h"
#include "..\Headers\ButtonUI.h"
#include "ImGuiHelper.h"
#include "KeyMgr.h"

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice), m_pKeyMgr(CKeyMgr::Get_Instance())
{
}

HRESULT CButtonUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButtonUI::ReadyGameObject(void * pArg/* = nullptr*/)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	UI_BUTTON_ADD_COMPONENT tagInput = *static_cast<UI_BUTTON_ADD_COMPONENT*>(pArg);;

	if (FAILED(AddComponent(&tagInput)))
		return E_FAIL;

	m_wsObjectName = tagInput.wsOnTexComTag;
	m_UIDesc.vUISize = tagInput.tUIDesc.vUISize;
	m_fMaxSize = m_UIDesc.vUISize.x;
	m_UIDesc.vUIPos = tagInput.tUIDesc.vUIPos;
	m_UIDesc.vCenter = tagInput.tUIDesc.vCenter;
	m_bTextOut = tagInput.bTextOut;
	m_function = tagInput.func;

	/*Convert to Window Loc*/
	_vector vConvertUIPos = m_UIDesc.vUIPos;
	vConvertUIPos.y = -m_UIDesc.vUIPos.y;
	vConvertUIPos.x += (float)(WINCX / 2);
	vConvertUIPos.y += (float)(WINCY / 2);

	m_tBoxRect.left = vConvertUIPos.x - (m_UIDesc.vUISize.x / 2);
	m_tBoxRect.top = vConvertUIPos.y - (m_UIDesc.vUISize.y / 2);
	m_tBoxRect.right = vConvertUIPos.x + (m_UIDesc.vUISize.x / 2);
	m_tBoxRect.bottom = vConvertUIPos.y + (m_UIDesc.vUISize.y / 2);

	if (!m_function)
		return E_FAIL;

	return S_OK;
}

_uint CButtonUI::UpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;

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



	//ImGui::Separator();
	//ImGui::SliderInt(" Max",
	//	reinterpret_cast<int*>(m_piMaxValue),
	//	0, 1000, "%d");
	//ImGui::Separator();
	//ImGui::SliderInt(" Min",
	//	reinterpret_cast<int*>(m_piMinValue),
	//	0, 1000, "%d");
	//ImGui::End();


	//m_UIDesc.vUISize.x = m_fMaxSize * m_fRatio;

	//누를 수 없는 버튼 - 버튼을 누르는 것은 Skip

	POINT tMousePt;
	GetCursorPos(&tMousePt);
	ScreenToClient(g_hWnd, &tMousePt);

	switch (m_eState)
	{
	case CButtonUI::EButtonState::DISABLED:
		//뭔가 다른 조건을 대입을 하면 다른 것으로 교체
		return _uint();
		break;
	case CButtonUI::EButtonState::ACTIVATION:
		//누를 수 있는 버튼
		if (m_pKeyMgr->Key_Down(VK_LBUTTON) &&
			m_tBoxRect.left <= tMousePt.x && m_tBoxRect.right >= tMousePt.x &&
			m_tBoxRect.top <= tMousePt.y && m_tBoxRect.bottom >= tMousePt.y)
		{
			m_function();
			m_eState = EButtonState::PUSH;
		}

		break;
	case CButtonUI::EButtonState::PUSH:
		//눌러졌음
		if (m_pKeyMgr->Key_Up(VK_LBUTTON))
		{
			m_eState = EButtonState::ACTIVATION;
		}
		break;
	default:
		return _uint();
		break;
	}

	return _uint();
}

_uint CButtonUI::LateUpdateGameObject(float fDeltaTime)
{
	if (!m_bShown)
		return S_OK;

	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CButtonUI::RenderGameObject()
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

	switch (m_eState)
	{
	case CButtonUI::EButtonState::DISABLED:
		//누를 수 없는 버튼
		if (FAILED(m_pTextureCom->Set_Texture(0)))
			return E_FAIL;
		break;
	case CButtonUI::EButtonState::ACTIVATION:
		//누를 수 있는 버튼
		if (FAILED(m_pOnTextureCom->Set_Texture(0)))
			return E_FAIL;
		break;
	case CButtonUI::EButtonState::PUSH:
		//눌러졌음
		if (FAILED(m_pPushTextureCom->Set_Texture(0)))
			return E_FAIL;
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButtonUI::AddComponent(UI_BUTTON_ADD_COMPONENT* _pInputTag)
{
	//TODO: 안에 있는 텍스쳐 편집
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		_pInputTag->wsOffTexProtoTag,
		_pInputTag->wsOffTexComTag,
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		_pInputTag->wsOnTexProtoTag,
		_pInputTag->wsOnTexComTag,
		(CComponent**)&m_pOnTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		_pInputTag->wsPushTexProtoTag,
		_pInputTag->wsPushTexComTag,
		(CComponent**)&m_pPushTextureCom)))
		return E_FAIL;
	return S_OK;
}

CButtonUI * CButtonUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CButtonUI* pInstance = new CButtonUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create PlyerInfoUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CButtonUI::Clone(void * pArg/* = nullptr*/)
{
	CButtonUI* pClone = new CButtonUI(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone PlyerInfoUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CButtonUI::Free()
{
	CGameUI::Free();
}
