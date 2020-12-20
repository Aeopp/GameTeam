#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "PlyerInfoUI.h"
#include "WeaponAmmoInfoUI.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CStage::ReadyScene()
{
	CCollisionComponent::CleanUpMapCollisionInfo();

	CScene::ReadyScene();

	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CMainCamera>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CMainCamera>(),
		reinterpret_cast<CGameObject**>(&_Camera), nullptr)))
		return E_FAIL;

	if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlayer>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CPlayer>(),
		(CGameObject**)&m_pPlayer, nullptr)))
		return E_FAIL;

	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlyerInfoUI>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CPlyerInfoUI>(),
		nullptr, nullptr)))
		return E_FAIL;

	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CWeaponAmmoInfoUI>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CWeaponAmmoInfoUI>(),
		nullptr, nullptr)))
		return E_FAIL;

	return S_OK;
}

_uint CStage::UpdateScene(float fDeltaTime)
{
	CScene::UpdateScene(fDeltaTime);

	return KeyProcess(fDeltaTime); 
}

_uint CStage::LateUpdateScene()
{
	CScene::LateUpdateScene();

	vec4 CameraLocation = (dynamic_cast<CStage*>
		(m_pManagement->GetCurrentScene())->_Camera->GetTransform()->GetLocation());
	CameraLocation.w = 1.f;

	vec4 LightLocation = (dynamic_cast<CStage*>
		(m_pManagement->GetCurrentScene())->m_pPlayer->GetTransform()->GetLocation());
	LightLocation.w = 1.f;

	Effect::Update(m_pDevice, CameraLocation, LightLocation);

	return _uint();
}

_uint CStage::KeyProcess(float fDeltaTime)
{

	if (ImGuiHelper::bEditOn && m_pManagement->bDebug)
	{
		ImGui::Begin("Information");
		ImGui::Text("Press L Key Is Third Person First Person Change");
		ImGui::Text("Press O Key Debug Toggle");
		ImGui::Text("Press L Key Edit Mode Toggle");
		ImGui::End();
	}

	if (m_pKeyMgr->Key_Down('P'))
	{
		ImGuiHelper::bEditOn = !ImGuiHelper::bEditOn;
	}
	if (m_pKeyMgr->Key_Down('O'))
	{
		m_pManagement->bDebug = !m_pManagement->bDebug;
	}
	if (m_pKeyMgr->Key_Down('L'))
	{
		_Camera->bThirdPerson = !_Camera->bThirdPerson;
	}

	PlayerKeyProcess(m_pPlayer ,fDeltaTime);



	return _uint();
}

void CStage::PlayerKeyProcess(CPlayer* const _CurrentPlayer, float fDeltaTime)
{
	static constexpr float DiagonalCorrection = 0.707f;

	if (m_pKeyMgr->Key_Pressing('W'))
	{
		if (m_pKeyMgr->Key_Pressing('A'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveRight(-fDeltaTime);
		}
		else if (m_pKeyMgr->Key_Pressing('D'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveRight(+fDeltaTime);
		}
		_CurrentPlayer->MoveForward(+fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Pressing('S'))
	{
		if (m_pKeyMgr->Key_Pressing('A'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveRight(-fDeltaTime);
		}
		else if (m_pKeyMgr->Key_Pressing('D'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveRight(+fDeltaTime);
		}
		_CurrentPlayer->MoveForward(-fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Pressing('A'))
	{
		if (m_pKeyMgr->Key_Pressing('W'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveForward(+fDeltaTime);
		}
		else if (m_pKeyMgr->Key_Pressing('S'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveForward(-fDeltaTime);
		}
		_CurrentPlayer->MoveRight(-fDeltaTime);
	}
	else if (m_pKeyMgr->Key_Pressing('D'))
	{
		if (m_pKeyMgr->Key_Pressing('W'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveForward(+fDeltaTime);
		}
		else if (m_pKeyMgr->Key_Pressing('S'))
		{
			fDeltaTime *= DiagonalCorrection;
			_CurrentPlayer->MoveForward(-fDeltaTime);
		}
		_CurrentPlayer->MoveRight(+fDeltaTime);
	}

	if (m_pKeyMgr->Key_Pressing('Z'))
	{
		auto& Desc = _CurrentPlayer->GetTransform()->m_TransformDesc;
		const mat world = Desc.matWorld;
		vec3 Up{ 0.f,1.f,0.f };
		Up = MATH::Normalize(Up);
		const float Speed = Desc.fSpeedPerSec;
		Desc.vPosition += Up * Speed * fDeltaTime;
	}
	if (m_pKeyMgr->Key_Pressing('X'))
	{
		auto& Desc = _CurrentPlayer->GetTransform()->m_TransformDesc;
		const mat world = Desc.matWorld;
		vec3 Down{ 0.f,-1.f,0.f };
		Down = MATH::Normalize(Down);
		const float Speed = Desc.fSpeedPerSec;
		Desc.vPosition += Down * Speed * fDeltaTime;
	}


	if (m_pKeyMgr->Key_Down(VK_LBUTTON))
	{
		m_pPlayer->MouseLeft();

		if (ImGuiHelper::bEditOn)
		{
			ImGuiHelper::Picking(m_pDevice, CCollisionComponent::GetMapPlaneInfo());	
		}
	}
	 if (m_pKeyMgr->Key_Down('R'))
	{
		m_pPlayer->RButtonEvent();
	}
	 if (m_pKeyMgr->Key_Down(VK_RBUTTON))
	{
		m_pPlayer->MouseRight();
	}
	 if (m_pKeyMgr->Key_Pressing(VK_RBUTTON))
	{
		m_pPlayer->MouseRightPressing();
	}
	 if (m_pKeyMgr->Key_Up(VK_RBUTTON))
	{
		m_pPlayer->MouseRightUp();
	}
	 if (m_pKeyMgr->Key_Down('1'))
	{
		m_pPlayer->_1ButtonEvent();
	}
	 if (m_pKeyMgr->Key_Down('2'))
	{
		m_pPlayer->_2ButtonEvent();
	}
	 if (m_pKeyMgr->Key_Down('3'))
	{
		m_pPlayer->_3ButtonEvent();
	}
	 if (m_pKeyMgr->Key_Down('4'))
	{
		m_pPlayer->_4ButtonEvent();
	}
	 if (m_pKeyMgr->Key_Down('5'))
	{
		m_pPlayer->_5ButtonEvent();
	}
	 if (m_pKeyMgr->Key_Pressing(VK_LBUTTON))
	{
		m_pPlayer->MouseLeftPressing();
	}
};


void CStage::Free()
{
	SafeRelease(m_pPlayer);
	SafeRelease(_Camera);
	SafeRelease(_CurrentMap);

	CScene::Free();
}
