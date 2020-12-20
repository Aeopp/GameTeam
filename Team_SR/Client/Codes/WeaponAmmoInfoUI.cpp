#include "stdafx.h"
#include "..\Headers\WeaponAmmoInfoUI.h"
#include "ImGuiHelper.h"
#include "MainCamera.h"
#include "Layer.h"

CWeaponAmmoInfoUI::CWeaponAmmoInfoUI(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	
}

HRESULT CWeaponAmmoInfoUI::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponAmmoInfoUI::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;
	
	m_pTransformCom->m_TransformDesc.vScale.x = 15.f;
	m_pTransformCom->m_TransformDesc.vScale.y = 15.f;
	m_pTransformCom->m_TransformDesc.vScale.z = 0;

	m_pTransformCom->m_TransformDesc.vPosition.x = 50.f;
	m_pTransformCom->m_TransformDesc.vPosition.y = -18.f;
	m_pTransformCom->m_TransformDesc.vPosition.z = 0.f;
	
	m_pTransformCom->m_TransformDesc.fRotatePerSec = 0.f;
	m_pTransformCom->m_TransformDesc.fSpeedPerSec = 0.f;

	return S_OK;
}

_uint CWeaponAmmoInfoUI::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);
	ImGui::Begin("WeaponAmmoInfoUI Edit");

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

_uint CWeaponAmmoInfoUI::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);


	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CWeaponAmmoInfoUI::RenderGameObject()
{
	auto camera = m_pManagement->GetGameObject((int)-1, CLayer::Tag + L"MainCamera", 0);
	if (nullptr == camera)
		return FALSE;

	// 2020 12 17 이호준
	// 뷰와 투영을 미리 저장 -> ( UI 를 렌더링 한 이후에 다시 세팅해주기 위함. ) 
	mat PrevView, PrevProjection;
	m_pDevice->GetTransform(D3DTS_VIEW, &PrevView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &PrevProjection);
	/// <summary>

	_matrix matScale, /*matRotX, matRotY, matRotZ,*/ matTrans, matOrthographic;

	TRANSFORM_DESC& tTransformDesc = m_pTransformCom->m_TransformDesc;

	//D3DXMatrixScaling(&matScale, tTransformDesc.vScale.x, tTransformDesc.vScale.y, tTransformDesc.vScale.z);
	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(tTransformDesc.vRotation.x));
	//D3DXMatrixRotationY(&matRotY, D3DXToRadian(tTransformDesc.vRotation.y));
	//D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tTransformDesc.vRotation.z));
	//D3DXMatrixTranslation(&matTrans, tTransformDesc.vPosition.x, tTransformDesc.vPosition.y, tTransformDesc.vPosition.z);

	//D3DXMatrixOrthoLH(&matOrthographic, WINCX, WINCY, 0.f, 1.f);
	//tTransformDesc.matWorld = matOrthographic* matScale * matTrans;
	//tTransformDesc.matWorld = matScale * matRotX * matRotY * matRotZ * matTrans
	


	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &matWorld)))
		return E_FAIL;

	_matrix matView;
	D3DXMatrixIdentity(&matView);
	
	matView._11 = tTransformDesc.vScale.x;
	matView._22 = tTransformDesc.vScale.y;
	matView._33 = 1.f;
	//matView._41 = tTransformDesc.vPosition.x;
	//matView._42 = tTransformDesc.vPosition.y;
	//matView._43 = tTransformDesc.vPosition.z;

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matView)))
		return E_FAIL;

	D3DXMatrixOrthoLH(&matOrthographic, WINCX, WINCY, 0.f, 1000.f);
	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrthographic)))
		return E_FAIL;

	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;
	
	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	// 2020 12 17 이호준
	// 미리 저장했던 뷰와 투영으로 렌더링 파이프라인에 다시 설정

	m_pDevice->SetTransform(D3DTS_VIEW, &PrevView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &PrevProjection);
	/// <summary>
	 
	//m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CWeaponAmmoInfoUI::AddComponent()
{
	/* For.Com_Texture */
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_WeaponAmmoInfoUI",
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



CWeaponAmmoInfoUI * CWeaponAmmoInfoUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWeaponAmmoInfoUI* pInstance = new CWeaponAmmoInfoUI(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create PlyerInfoUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CWeaponAmmoInfoUI::Clone(void * pArg)
{
	CWeaponAmmoInfoUI* pClone = new CWeaponAmmoInfoUI(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone PlyerInfoUI");
		SafeRelease(pClone);
	}

	return pClone;
}

void CWeaponAmmoInfoUI::Free()
{
	CGameObject::Free();
}
