#include "stdafx.h"
#include "..\Headers\Stage1st.h"
#include "Player.h"
#include "MainCamera.h"
#include "Layer.h"
#include "Map1st.h"
#include "Glacier.h"
#include "BatGrey.h"
#include "PlyerInfoUI.h"
#include "Spider.h"

#include "Eyebat.h"
CStage1st::CStage1st(LPDIRECT3DDEVICE9 pDevice)
	: Super(pDevice)
{
}

HRESULT CStage1st::ReadyScene()
{
	Super::ReadyScene();

	{
		const wstring GameObjTag = CGameObject::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectPrototype(
			(_int)ESceneID::Stage1st,
			GameObjTag,
			CMap1st::Create(m_pDevice))))
			return E_FAIL;

		const std::wstring LayerTag = CLayer::Tag + TYPE_NAME<CMap1st>();

		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Stage1st,
			GameObjTag,
			(_int)ESceneID::Stage1st,
			LayerTag,
			reinterpret_cast<CGameObject**>(&_CurrentMap), nullptr)))
			return E_FAIL;

		std::wifstream In(L"..\\Resources\\BatGrey.txt");
		std::wstring str;
		vec3 Location;
		vec3 Rotation;
		vec3 Scale;
		while (In)
		{
			std::getline(In, str);
			std::wstringstream wss(str);
			std::wstring Token;
			wss >> Token;

			std::wstring Name;

			if (Token == L"Name")
			{
				wss >> Name;
			}
			else if (Token == L"Location")
			{
				wss >> Location.x;
				wss >> Location.y;
				wss >> Location.z;
			}
			else if (Token == L"Scale")
			{
				wss >> Scale.x;
				wss >> Scale.y;
				wss >> Scale.z;
			}
			else if (Token == L"Rotation")
			{
				wss >> Rotation.x;
				wss >> Rotation.y;
				wss >> Rotation.z;
			}

			/*if (Name == L"BatGrey")
			{
				MonsterBasicArgument stArg;
				stArg.uiSize = sizeof(MonsterBasicArgument);
				stArg.pPlayer = m_pPlayer;
				stArg.vPosition = Location;
				if (FAILED(m_pManagement->AddGameObjectInLayer(
					(_int)ESceneID::Static,
					CGameObject::Tag + TYPE_NAME<CBatGrey>(),
					(_int)ESceneID::Stage1st,
					CLayer::Tag + TYPE_NAME<CMonster>(),
					nullptr, static_cast<void*>(&stArg))))
					return E_FAIL;
			}
			else if (Name == L"Glacier")
			{
				MonsterBasicArgument stArg;
				stArg.uiSize = sizeof(MonsterBasicArgument);
				stArg.pPlayer = m_pPlayer;
				stArg.vPosition = Location;
				if (FAILED(m_pManagement->AddGameObjectInLayer(
					(_int)ESceneID::Static,
					CGameObject::Tag + TYPE_NAME<CGlacier>(),
					(_int)ESceneID::Stage1st,
					CLayer::Tag + TYPE_NAME<CMonster>(),
					nullptr, static_cast<void*>(&stArg))))
					return E_FAIL;
			}*/
		};
		static constexpr float RandRange = 50;
		MonsterBasicArgument stArg;
		stArg.uiSize = sizeof(MonsterBasicArgument);
		stArg.pPlayer = m_pPlayer;
		stArg.vPosition = { -20.f, 10.f, 20 };
		//stArg.vPosition = { -5.f, 10.f, 20 };
		//if (FAILED(m_pManagement->AddGameObjectInLayer(
		//	(_int)ESceneID::Static,
		//	CGameObject::Tag + TYPE_NAME<CSpider>(),
		//	(_int)ESceneID::Stage1st,
		//	CLayer::Tag + TYPE_NAME<CMonster>(),
		//	nullptr, static_cast<void*>(&stArg))))
		//	return E_FAIL;


		//if (FAILED(m_pManagement->AddGameObjectInLayer(
		//	(_int)ESceneID::Static,
		//	CGameObject::Tag + TYPE_NAME<CGlacier>(),
		//	(_int)ESceneID::Stage1st,
		//	CLayer::Tag + TYPE_NAME<CMonster>(),
		//	nullptr, static_cast<void*>(&stArg))))
		//	return E_FAIL;

		//stArg.vPosition = { MATH::RandReal({-RandRange ,RandRange }), 10.f, MATH::RandReal({-RandRange ,RandRange }) };
			//if (FAILED(m_pManagement->AddGameObjectInLayer(
			//	(_int)ESceneID::Static,
			//	CGameObject::Tag + TYPE_NAME<CBatGrey>(),
			//	(_int)ESceneID::Stage1st,
			//	CLayer::Tag + TYPE_NAME<CMonster>(),
			//	nullptr, static_cast<void*>(&stArg))))
			//	return E_FAIL;

			// 글레이서
			//stArg.vPosition = { 5.f, 10.f, 30.f };
		
			// 눈깔박쥐
			//stArg.vPosition = { MATH::RandReal({-RandRange ,RandRange }), 10.f, MATH::RandReal({-RandRange ,RandRange }) };

		if (FAILED(m_pManagement->AddGameObjectInLayer(
			(_int)ESceneID::Static,
			CGameObject::Tag + TYPE_NAME<CEyebat>(),
			(_int)ESceneID::Stage1st,
			CLayer::Tag + TYPE_NAME<CMonster>(),
			nullptr, static_cast<void*>(&stArg))))
			return E_FAIL;

	}


	return S_OK;
}

_uint CStage1st::UpdateScene(float fDeltaTime)
{
	Super::UpdateScene(fDeltaTime);

	return _uint();
}

_uint CStage1st::LateUpdateScene()
{
	Super::LateUpdateScene();

	return _uint();
}

_uint CStage1st::KeyProcess(float fDeltaTime)
{
	Super::KeyProcess(fDeltaTime);

	return _uint();
}

void CStage1st::PlayerKeyProcess(CPlayer* const _CurrentPlayer,  float fDeltaTime)
{
	Super::PlayerKeyProcess(_CurrentPlayer, fDeltaTime);

}

CStage1st* CStage1st::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CStage1st* pInstance = new CStage1st(pDevice);
	if (FAILED(pInstance->ReadyScene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStage");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CStage1st::Free()
{
	Super::Free();
}
