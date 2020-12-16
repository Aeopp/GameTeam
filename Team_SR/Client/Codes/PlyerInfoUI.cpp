#include "stdafx.h"
#include "..\Headers\PlyerInfoUI.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "Layer.h"

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
	m_pTransformCom->m_TransformDesc.vScale.x = 200;
	m_pTransformCom->m_TransformDesc.vScale.y = 200;
	m_pTransformCom->m_TransformDesc.vScale.z = 0;

	m_pTransformCom->m_TransformDesc.vPosition.x = 1;
	m_pTransformCom->m_TransformDesc.vPosition.y = 1;
	m_pTransformCom->m_TransformDesc.vPosition.z = 0;
	
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
		-1000.f, +1000.f, "%f");

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


	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CPlyerInfoUI::RenderGameObject()
{
	_matrix matWorld;

	auto camera = m_pManagement->GetGameObject((int)ESceneID::Stage1st, CLayer::Tag + L"MainCamera", 0);
	if (nullptr == camera)
		return FALSE;


	_matrix matScale, /*matRotX, matRotY, matRotZ,*/ matTrans, matOrthographic;

	TRANSFORM_DESC& tTransformDesc = m_pTransformCom->m_TransformDesc;

	D3DXMatrixScaling(&matScale, tTransformDesc.vScale.x, tTransformDesc.vScale.y, tTransformDesc.vScale.z);
	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(tTransformDesc.vRotation.x));
	//D3DXMatrixRotationY(&matRotY, D3DXToRadian(tTransformDesc.vRotation.y));
	//D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tTransformDesc.vRotation.z));
	D3DXMatrixTranslation(&matTrans, tTransformDesc.vPosition.x, tTransformDesc.vPosition.y, tTransformDesc.vPosition.z);

	D3DXMatrixOrthoLH(&matOrthographic, WINCX, WINCY, 0.f, 1.f);
	tTransformDesc.matWorld = matOrthographic* matScale * matTrans;
	//tTransformDesc.matWorld = matScale * matRotX * matRotY * matRotZ * matTrans
	
	D3DXMatrixIdentity(&matWorld);

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &matWorld)))
		return E_FAIL;

	_matrix matView;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matScale)))
		return E_FAIL;

	_matrix matProj;
	
	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrthographic)))
		return E_FAIL;

	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;
	
	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

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
