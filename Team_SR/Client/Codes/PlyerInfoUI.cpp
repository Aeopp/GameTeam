#include "stdafx.h"
#include "..\Headers\PlyerInfoUI.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"

CPlyerInfoUI::CPlyerInfoUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	
}

HRESULT CPlyerInfoUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlyerInfoUI::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;
	m_pTransformCom->m_TransformDesc.vScale.x = 1;
	m_pTransformCom->m_TransformDesc.vScale.y = 1;
	m_pTransformCom->m_TransformDesc.vScale.z = 1;

	m_pTransformCom->m_TransformDesc.vPosition.x = 1;
	m_pTransformCom->m_TransformDesc.vPosition.y = 1;
	m_pTransformCom->m_TransformDesc.vPosition.z = 1;
	
	m_pTransformCom->m_TransformDesc.fRotatePerSec = 0.f;
	m_pTransformCom->m_TransformDesc.fSpeedPerSec = 0.f;
	
	

	return S_OK;
}

_uint CPlyerInfoUI::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);
	ImGui::Begin("PlayerInfoUI Edit");

	ImGui::Separator();
	ImGui::SliderFloat3("Scale",
		reinterpret_cast<float*>(&m_pTransformCom->m_TransformDesc.vScale),
		-100.f, +100.f, "%f");

	ImGui::Separator();
	ImGui::SliderFloat3("Location",
		reinterpret_cast<float*>(&m_pTransformCom->m_TransformDesc.vPosition),
		-100.f, +100.f, "%f");

	ImGui::End();

	return _uint();
}

_uint CPlyerInfoUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);


	auto pCamera = (CMainCamera*)m_pManagement->GetGameObject((_int)ESceneID::Stage1st, CGameObject::Tag + L"MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;
	auto cameraDesc = pCamera->GetCameraDesc();
	_matrix matView, matProj;
	D3DXMatrixLookAtLH(&matView, &cameraDesc.vEye, &cameraDesc.vAt, &cameraDesc.vUp);
	D3DXMatrixPerspectiveFovLH(&matProj, cameraDesc.fFovY, cameraDesc.fAspect, cameraDesc.fNear, cameraDesc.fFar);

	m_pTransformCom->m_TransformDesc.matWorld = matView * matProj;

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CPlyerInfoUI::RenderGameObject()
{
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	

	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

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

	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Engine::CVIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
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
	CPlyerInfoUI* pClone = new CPlyerInfoUI(*this); /* 복사생성자 */
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
	CGameObject::Free();
}
