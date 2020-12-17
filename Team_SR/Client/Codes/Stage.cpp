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
	CScene::ReadyScene();

	{
		if (FAILED(m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CPlayer>(),
			(_int)ESceneID::Stage1st,
			CLayer::Tag + TYPE_NAME<CPlayer>(),
			(CGameObject**)&m_pPlayer,nullptr)))
			return E_FAIL;
	}

	{
		const std::wstring Type = TYPE_NAME<CMainCamera>();
		const std::wstring GameObjTag = CGameObject::Tag + Type ;
		const std::wstring LayerTag = CLayer::Tag + Type;

		if (FAILED ( m_pManagement->AddGameObjectInLayer((_int)ESceneID::Static,
			GameObjTag,
			(_int)ESceneID::Stage1st,
			LayerTag,
			reinterpret_cast<CGameObject**>(&_Camera), nullptr)))
		{
			return E_FAIL;
		}
	}

	//UI
	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CPlyerInfoUI>(),
		(_int)ESceneID::Stage1st,
		CLayer::Tag + TYPE_NAME<CPlyerInfoUI>(),
		nullptr, nullptr)))
		return E_FAIL;

	if (FAILED(m_pManagement->AddGameObjectInLayer(
		(_int)ESceneID::Static,
		CGameObject::Tag + TYPE_NAME<CWeaponAmmoInfoUI>(),
		(_int)ESceneID::Stage1st,
		CLayer::Tag + TYPE_NAME<CWeaponAmmoInfoUI>(),
		nullptr, nullptr)))
		return E_FAIL;
	return S_OK;
}

_uint CStage::UpdateScene(float fDeltaTime)
{
	CScene::UpdateScene(fDeltaTime);

	KeyProcess(fDeltaTime);

	return _uint();
}

_uint CStage::LateUpdateScene()
{
	CScene::LateUpdateScene();

	return _uint();
}

_uint CStage::KeyProcess(float fDeltaTime)
{
	if (m_pKeyMgr->Key_Down('P'))
	{
		ImGuiHelper::bEditOn = !ImGuiHelper::bEditOn;
	}

	PlayerKeyProcess(m_pPlayer ,fDeltaTime);

	return _uint();
}

void CStage::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
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

	// REMOVEPLZ........
	if (m_pKeyMgr->Key_Pressing('Z'))
	{
		auto& Desc = _CurrentPlayer->GetTransform()->m_TransformDesc;
		const mat world = Desc.matWorld;
		vec3 Up{ 0.f,1.f,0.f};
		Up = MATH::Normalize(Up);
		const float Speed = Desc.fSpeedPerSec;
		Desc.vPosition += Up * Speed * fDeltaTime;
	}
	else if (m_pKeyMgr->Key_Pressing('X'))
	{
		auto& Desc = _CurrentPlayer->GetTransform()->m_TransformDesc;
		const mat world = Desc.matWorld;
		vec3 Down{ 0.f,-1.f,0.f };
		Down = MATH::Normalize(Down);
		const float Speed = Desc.fSpeedPerSec;
		Desc.vPosition += Down * Speed * fDeltaTime;
	}
	
	if (m_pKeyMgr->Key_Pressing(VK_LBUTTON))
	{
		POINT _MousePt;
		GetCursorPos(&_MousePt);
		ScreenToClient(g_hWnd, &_MousePt);
		vec3 _MouseVec { static_cast<float>(_MousePt.x),static_cast<float>(_MousePt.y),0.f };
		Ray _Ray =MATH::GetRayScreenProjection(_MouseVec, m_pDevice,
			static_cast<float>(WINCX),static_cast<float>( WINCY));
		m_pPlayer->_CollisionComp->_Ray = std::move(_Ray);
	}
	////////////
}



void CStage::Free()
{
	SafeRelease(m_pPlayer);
	SafeRelease(_Camera);
	SafeRelease(_CurrentMap);

	CScene::Free();
}
