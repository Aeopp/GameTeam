#include "stdafx.h"
#include "..\Headers\PlyerInfoUI.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "Layer.h"
#include "VIBuffer_UITexture.h"

CPlyerInfoUI::CPlyerInfoUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameUI(pDevice)
{
	
}

HRESULT CPlyerInfoUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameUI::ReadyGameObjectPrototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPlyerInfoUI::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameUI::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;
	
	m_UIDesc.vUISize.x = WINCX / 3.f;
	m_UIDesc.vUISize.y = WINCY / 2.f;
	m_UIDesc.vUISize.z = 0;

	m_UIDesc.vUIPos.x = -(WINCX / 2);
	m_UIDesc.vUIPos.y = -(WINCY / 2);
	m_UIDesc.vUIPos.z = 2.f;

	m_UIDesc.vCenter = _vector(-1.f, -1.f, 0.f);

	return S_OK;
}

_uint CPlyerInfoUI::UpdateGameObject(float fDeltaTime)
{
	CGameUI::UpdateGameObject(fDeltaTime);
	ImGui::Begin("PlayerInfoUI Edit");

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

_uint CPlyerInfoUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameUI::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CPlyerInfoUI::RenderGameObject()
{
<<<<<<< HEAD
	if (FAILED(CGameUI::RenderGameObject()))
		return E_FAIL;
=======
	//CGameUI::RenderGameObject();
>>>>>>> origin/main

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_UIDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_UIDesc.matView)))
	//	return E_FAIL;

	//if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_UIDesc.matOrthographic)))
	//	return E_FAIL;

<<<<<<< HEAD
	//if (FAILED(CGameUI::RenderGameObject()))
=======
	//if (FAILED(CGameObject::RenderGameObject()))
>>>>>>> origin/main
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(0)))
	//	return E_FAIL;
	//
	////m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	////m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CPlyerInfoUI::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_PlayerInfoUI",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}



CPlyerInfoUI * CPlyerInfoUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CPlyerInfoUI* pInstance = new CPlyerInfoUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create PlyerInfoUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CPlyerInfoUI::Clone(void * pArg)
{
	CPlyerInfoUI* pClone = new CPlyerInfoUI(*this); /* ��������� */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone PlyerInfoUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CPlyerInfoUI::Free()
{
	CGameUI::Free();
}
