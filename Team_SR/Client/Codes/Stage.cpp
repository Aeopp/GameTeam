#include "stdafx.h"
#include "..\Headers\Stage.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "PlayerInfoUI.h"
#include "WeaponAmmoInfoUI.h"
#include "UIManager.h"
#include "Eyebat.h"
#include "Glacier.h"
#include "ParticleSystem.h"
#include "ScreenEffect.h"
#include "MiniMap.h"


#include "Hangman.h"
#include "Hellhound.h"

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
		CGameObject::Tag + TYPE_NAME<CScreenEffect>(),
		(_int)CurrentSceneID,
		CLayer::Tag + TYPE_NAME<CScreenEffect>(),
		nullptr, nullptr)))
		return E_FAIL;

	CUIManager::Get_Instance()->OnAllUI();

	return S_OK;
}

_uint CStage::UpdateScene(float fDeltaTime)
{
	KeyProcess(fDeltaTime);
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_STAGE1.wav", CSoundMgr::BGM);
	return 	CScene::UpdateScene(fDeltaTime);
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
	PlayerKeyProcess(m_pPlayer, fDeltaTime);



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

	if (m_pKeyMgr->Key_Down('M'))
	{
		dynamic_cast<CMiniMap* const>(m_pManagement->GetGameObject(-1, L"Layer_MiniMap", 0))->MapOpenToggle();
	};

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
	 if (m_pKeyMgr->Key_Up(VK_LBUTTON))
	 {
		 m_pPlayer->MouseLeftUp();
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
	 if (m_pKeyMgr->Key_Down('6'))
	 {
		 m_pPlayer->_6ButtonEvent();
	 }
	 if (m_pKeyMgr->Key_Down('7'))
	 {
		 m_pPlayer->_7ButtonEvent();
	 }
	 if (m_pKeyMgr->Key_Down('8'))
	 {
		 m_pPlayer->_8ButtonEvent();
	 }

	 if (m_pKeyMgr->Key_Down('Q'))
	 {
		 m_pPlayer->SpellFreeze();
	 }

	 if (m_pKeyMgr->Key_Down('E'))
	 {
		 m_pPlayer->SpellLight();
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



void CStage::LoadObjects(const std::wstring& FilePath,
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
			{ LocalPoint.x * WorldScale.x ,
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
};
void CStage::SpawnObjectFromName(const std::wstring& ObjectName, vec3 SpawnLocation) & noexcept
{
	// 박쥐
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
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 글레이서
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
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 행맨
	if (ObjectName.find(L"Hangman") != std::wstring::npos)
	{
		using SpawnType = CHangman;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 헬 하운드
	if (ObjectName.find(L"Hellhound") != std::wstring::npos)
	{
		using SpawnType = CHellhound;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 스파이더
	if (ObjectName.find(L"Spider") != std::wstring::npos)
	{
		using SpawnType = CHellhound;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 눈깔 박쥐
	if (ObjectName.find(L"BatGrey") != std::wstring::npos)
	{
		using SpawnType = CHellhound;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
	}

	// 보스 샤크
	if (ObjectName.find(L"Shark") != std::wstring::npos)
	{
		using SpawnType = CHellhound;

		MonsterBasicArgument _MonsterBasicArgument;
		_MonsterBasicArgument.uiSize = sizeof(MonsterBasicArgument);
		_MonsterBasicArgument.pPlayer = m_pPlayer;
		_MonsterBasicArgument.vPosition = std::move(SpawnLocation);

		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<SpawnType>(),
			(int)CurrentSceneID,
			CLayer::Tag + L"Monster", nullptr, &_MonsterBasicArgument);
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


#pragma region 장식들
	// 횃불
	if (ObjectName.find(L"Torch") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Torch;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 양초
	if (ObjectName.find(L"Candle") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Candle;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 나무 통
	if (ObjectName.find(L"Barrel") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Barrel;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 폭탄 통
	if (ObjectName.find(L"BarrelBomb") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::BarrelBomb;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 폐기물 통
	if (ObjectName.find(L"BarrelWaste") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::BarrelWaste;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 나무 상자 1
	if (ObjectName.find(L"Box1") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Box1;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 나무 상자 2
	if (ObjectName.find(L"Box2") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Box2;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 강철 상자 1
	if (ObjectName.find(L"BoxSteel1") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::BoxSteel1;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 강철 상자 2
	if (ObjectName.find(L"BoxSteel2") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::BoxSteel2;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 묘비 1
	if (ObjectName.find(L"Headstone1") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Headstone1;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 묘비 2
	if (ObjectName.find(L"Headstone2") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Headstone2;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 묘비 3
	if (ObjectName.find(L"Headstone3") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Headstone3;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 가시 덩굴
	if (ObjectName.find(L"ThornyVine") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::ThornyVine;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 나무
	if (ObjectName.find(L"TreeBlight") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::TreeBlight;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 나무 1
	if (ObjectName.find(L"Tree1") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Tree1;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 나무 2
	if (ObjectName.find(L"Tree2") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Tree2;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 시체 1
	if (ObjectName.find(L"Dead_Caleb") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Dead_Caleb;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 시체 2
	if (ObjectName.find(L"Dead_Doomguy") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Dead_Doomguy;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 시체 3
	if (ObjectName.find(L"Dead_Duke") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Dead_Duke;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 시체 4
	if (ObjectName.find(L"Dead_Sam") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Dead_Sam;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

	// 죽은 시체 5
	if (ObjectName.find(L"Dead_Wang") != std::wstring::npos)
	{
		DecoratorBasicArgument stDecoArg;
		stDecoArg.vPosition = std::move(SpawnLocation);
		stDecoArg.eType = DECO::Dead_Wang;
		m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + L"Decorator",
			(_int)CurrentSceneID,
			CLayer::Tag + L"Decorator",
			nullptr, static_cast<void*>(&stDecoArg));
	}

#pragma endregion
};
