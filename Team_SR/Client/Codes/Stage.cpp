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
#include "UIManager.h"
#include "Eyebat.h"
#include "Glacier.h"
#include "ParticleSystem.h"


CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice), m_pUIManager(CUIManager::Get_Instance())
{
	SafeAddRef(m_pUIManager);
}

HRESULT CStage::ReadyScene()
{
	CCollisionComponent::CleanUpMapCollisionInfo();
	ParticleSystem::Instance().ClearParticle();

	// 2020.12.22 00:01 KMJ
	// �����ʿ��� ���� �� �˱� ���ؼ�
	m_iSceneIndex = static_cast<int>(CurrentSceneID);

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
	SafeRelease(m_pUIManager);

	CScene::Free();
}



void CStage::LoadObjects(const std::wstring& FilePath ,
	 const vec3 WorldScale) & noexcept
{
	struct ObjectSpawnInfo
	{
		vec3 Location{ 0,0,0 };
		std::wstring Name{};
	};
	std::vector<ObjectSpawnInfo> _ObjectSpawnInfos;

	std::wifstream InputStream(FilePath);

	if (!InputStream.is_open())
	{
		PRINT_LOG(L"Warning!", L"Not Find Map Object Spawn Information");
	}

	std::wstring Token;
	std::vector<vec3> AccumulateWorldPoint{};

	while (InputStream)
	{
		Token.clear();
		InputStream >> Token;

		if (Token == L"v")
		{
			vec3 LocalPoint;
			InputStream >> LocalPoint.x;
			InputStream >> LocalPoint.y;
			InputStream >> LocalPoint.z;

			const vec3 WorldPoint =
			{   LocalPoint.x * WorldScale.x ,
				LocalPoint.y * WorldScale.y ,
				LocalPoint.z * WorldScale.z };

			AccumulateWorldPoint.push_back(WorldPoint);
		}

		if (Token == L"o")
		{
			ObjectSpawnInfo _CurrentObjectInfo;

			InputStream >> _CurrentObjectInfo.Name;

			_CurrentObjectInfo.Location =
				std::accumulate(std::begin(AccumulateWorldPoint), std::end(AccumulateWorldPoint), vec3{ 0,0,0 })
				/
				static_cast<float>(AccumulateWorldPoint.size());

			AccumulateWorldPoint.clear();
			AccumulateWorldPoint.shrink_to_fit();

			_ObjectSpawnInfos.push_back(std::move(_CurrentObjectInfo));
		}
	}

	for (auto& _CurrentObjectSpawnInfo : _ObjectSpawnInfos)
	{
		SpawnObjectFromName(_CurrentObjectSpawnInfo.Name, _CurrentObjectSpawnInfo.Location);
	}
}
void CStage::SpawnObjectFromName(const std::wstring& ObjectName, vec3 SpawnLocation) & noexcept
{
	if (ObjectName.find(L"BatGrey") != std::wstring::npos)
	{
		using SpawnType = CEyebat;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + TYPE_NAME<SpawnType>(), nullptr, &_MonsterBasicArgument);
	}

	if (ObjectName.find(L"Glacier") != std::wstring::npos)
	{
		using SpawnType = CGlacier;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + TYPE_NAME<SpawnType>(), nullptr, &_MonsterBasicArgument);
	}

	
	if (ObjectName.find(L"HealthBig") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::HealthBig;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}

	if (ObjectName.find(L"HealthSmall") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::HealthSmall;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}

	if (ObjectName.find(L"ManaBig") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::ManaBig;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}

	if (ObjectName.find(L"ManaSmall") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::ManaSmall;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}


	if (ObjectName.find(L"Ammo") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::Ammo;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}

	if (ObjectName.find(L"KeyBlue") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::KeyBlue;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	}


	if (ObjectName.find(L"KeyRed") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::KeyRed;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	};

	if (ObjectName.find(L"KeyYellow") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::KeyYellow;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	};
	if (ObjectName.find(L"Upgrade") != std::wstring::npos)
	{
		ItemBasicArgument stItemArg;
		stItemArg.uiSize = sizeof(ItemBasicArgument);
		stItemArg.vPosition = std::move(SpawnLocation);
		stItemArg.etype = ITEM::Upgrade;
		stItemArg.bDeleteFlag = false;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Item",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Item",
			nullptr, static_cast<void*>(&stItemArg));
	};

};
