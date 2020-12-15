#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"

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
}



void CStage::Free()
{
	SafeRelease(m_pPlayer);
	SafeRelease(_Camera);
	SafeRelease(_CurrentMap);

	CScene::Free();
}
