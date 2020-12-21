#include "stdafx.h"
#include "..\Headers\StateBar.h"
#include "Management.h"

CStateBar::CStateBar(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

HRESULT CStateBar::ReadyComponentPrototype()
{
	return S_OK;
}

HRESULT CStateBar::ReadyComponent(void * pArg/* = nullptr*/)
{
	return S_OK;
}

HRESULT CStateBar::test()
{
	CManagement* pManagement = CManagement::Get_Instance();

//#pragma region Component_Texture_StateBar
//	if (FAILED(pManagement->AddComponentPrototype(
//		(_int)ESceneID::Static,
//		L"Component_Texture_PlayerInfoUI",
//		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/UI/HUD/HUD_bottom_left.png", 1))))
//		return E_FAIL;
//#pragma endregion

	return E_NOTIMPL;
}

CStateBar * CStateBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStateBar* pInstance = new CStateBar(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create StateBar");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CStateBar::Clone(void * pArg/* = nullptr*/)
{
	CStateBar* pClone = new CStateBar(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone StateBar");
		SafeRelease(pClone);
	}
	return pClone;
}

void CStateBar::Free()
{
	CComponent::Free();
}
