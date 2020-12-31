#include "stdafx.h"
#include "..\Headers\Player.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "Vertexs.h"
#include "Monster.h"
#include "DXWrapper.h"
#include "NormalUVVertexBuffer.h"
#include "ParticleSystem.h"
#include "MainCamera.h"
#include "Item.h"
#include "ScreenEffect.h"
#include "boost/algorithm/clamp.hpp"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{}

HRESULT CPlayer::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	// Harvester 로딩
	{
		_AnimationTextures._TextureMap[L"ShotGun_Shot"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\ShotGun\\Shot\\", 2);

		_AnimationTextures._TextureMap[L"ShotGun_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\ShotGun\\Idle\\", 1);

		/*_AnimationTextures._TextureMap[L"Harvester_Hud"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Harvester\\Hud\\", 1);*/

		_AnimationTextures._TextureMap[L"ShotGun_Reload"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\ShotGun\\Reload\\", 14);
	}

	{
		_AnimationTextures._TextureMap[L"Dagger_Stab"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Stab\\", 4);

		_AnimationTextures._TextureMap[L"Dagger_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Idle\\", 1);

		/*_AnimationTextures._TextureMap[L"Dagger_Hud"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Hud\\", 1);*/

		_AnimationTextures._TextureMap[L"Dagger_Throw"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Throw\\", 12);

		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX, 1);
	}

	{
		_AnimationTextures._TextureMap[L"Magnum_Fire"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Fire\\", 4);

		_AnimationTextures._TextureMap[L"Magnum_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Idle\\", 1);

		/*_AnimationTextures._TextureMap[L"Magnum_Hud"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Hud\\", 1);*/
	}

	{
		_AnimationTextures._TextureMap[L"Akimbo_Fire"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Fire\\", 4);

		_AnimationTextures._TextureMap[L"Akimbo_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Idle\\", 1);

		/*_AnimationTextures._TextureMap[L"Akimbo_Hud"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Hud\\", 1);*/
	}

	{
		_AnimationTextures._TextureMap[L"Staff_Fire"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Fire\\", 4);

	/*	_AnimationTextures._TextureMap[L"Staff_Hud"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Hud\\", 1);*/

		_AnimationTextures._TextureMap[L"Staff_Charge"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Charge\\", 16);

		_AnimationTextures._TextureMap[L"Staff_Loop"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Loop\\", 10);

		_AnimationTextures._TextureMap[L"Staff_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Staff_Release"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Staff\\ChargeFire\\", 5);
	}

	{
		_AnimationTextures._TextureMap[L"Dynamite_Idle"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dynamite\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Dynamite_Throw"] = CreateTexturesSpecularNormal(
			m_pDevice, L"..\\Resources\\Player\\Dynamite\\Throw\\", 15ul);
	};

	{
		_AnimationTextures._TextureMap[L"ElectricStaff_Idle"] = CreateTexturesSpecularNormal
			(	m_pDevice, L"..\\Resources\\Player\\ElectricStaff\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"ElectricStaff_Fire"] = CreateTexturesSpecularNormal
			(	m_pDevice, L"..\\Resources\\Player\\ElectricStaff\\Fire\\", 6ul);
	};

	{
		_AnimationTextures._TextureMap[L"Flak_Idle"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Player\\Flak\\Idle\\", 1ul);

		_AnimationTextures._TextureMap[L"Flak_Fire"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Player\\Flak\\Fire\\", 4ul);

		_AnimationTextures._TextureMap[L"Flak_Reload"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Player\\Flak\\Reload\\", 15ul);
	};

	{
		_AnimationTextures._TextureMap[L"Light"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Player\\Light\\", 5ul);
	};

	{
		_AnimationTextures._TextureMap[L"Freeze"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Player\\Freeze\\", 11ul);


	};

	{
		_WeaponEffectAnimTextures._TextureMap[L"Electric_Heavy"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Effect\\ElectricHeavy\\", 9ul);

		_WeaponEffectAnimTextures._TextureMap[L"Electric_Medium"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Effect\\ElectricMedium\\", 9ul);

		_WeaponEffectAnimTextures._TextureMap[L"Electric_Light"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Effect\\ElectricLight\\", 9ul);

		_WeaponEffectAnimTextures._TextureMap[L"Electric_Blast"] = CreateTexturesSpecularNormal
		(m_pDevice, L"..\\Resources\\Effect\\ElectricBlast\\", 12ul);
	}

	return S_OK;
}

HRESULT CPlayer::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (pArg)
	{
		ESceneID*  _SceneIDPtr = reinterpret_cast<ESceneID*>(pArg);
		switch (*_SceneIDPtr)
		{
		case ESceneID::StageMidBoss:
		case ESceneID::StageFinalBoss:
			ShieldStart();
			break;
		default:
			break;
		}
	};

	m_pTransformCom->m_TransformDesc.fSpeedPerSec = 23.f;
	m_pTransformCom->m_TransformDesc.fRotatePerSec = MATH::PI;
	m_pTransformCom->m_TransformDesc.vPosition = { 0,10,0 };
	m_pTransformCom->m_TransformDesc.vRotation = { 0,0,0 };
	m_pTransformCom->m_TransformDesc.vScale = { 1,1,1 };

	
	return S_OK;
};

_uint CPlayer::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);
	_DeltaTime = fDeltaTime;

	if (ImGuiHelper::bEditOn)
	{
		ImGui::Begin("PlayerEdit", &ImGuiHelper::bEditOn);
		ImGui::Text("Rotation");
		ImGui::SliderFloat("Pitch", &m_pTransformCom->m_TransformDesc.vRotation.x, -360, 360, "Deg : %f");
		ImGui::SliderFloat("Yaw", &m_pTransformCom->m_TransformDesc.vRotation.y, -360, 360, "Deg : %f");
		ImGui::SliderFloat("Roll", &m_pTransformCom->m_TransformDesc.vRotation.z, -360, 360, "Deg : %f");

		ImGui::Text("Scale");
		ImGui::Separator();
		ImGui::SliderFloat("x", &m_pTransformCom->m_TransformDesc.vScale.x, 0.1, 100.f);
		ImGui::SliderFloat("y", &m_pTransformCom->m_TransformDesc.vScale.y, 0.1, 100.f);
		ImGui::SliderFloat("z", &m_pTransformCom->m_TransformDesc.vScale.z, 0.1, 100.f);

		ImGui::Separator();
		ImGui::Text("Information");
		ImGui::Separator();
		ImGui::Text("Location");
		const auto& Location = m_pTransformCom->m_TransformDesc.vPosition;
		ImGui::Text("X : %f", Location.x);
		ImGui::Text("Y : %f", Location.y);
		ImGui::Text("Z : %f", Location.z);

		ImGui::Text("Collision");
		ImGui::Checkbox("Collision?", &_CollisionComp->bCollision);

		ImGui::End();
	}

	_CollisionComp->Update(m_pTransformCom);
	_AnimationTextures.Update(fDeltaTime);
	_WeaponEffectAnimTextures.Update(fDeltaTime);
	for (auto& _LightDurationPair : LightingDurationTable)
	{
		float& _Duration = _LightDurationPair.second;
		_Duration -= fDeltaTime;
		if (_Duration > 0.0f)
		{
			PushLightFromName(_LightDurationPair.first ,_LightDurationPair.second);
		}
	};

	MyLight _Light;
	_Light.Diffuse = { 1,1,1,1 };
	_Light.Location = MATH::ConvertVec4(m_pTransformCom->GetLocation(), 1.f);
	_Light.Radius = 30.f;
	_Light.Priority = 0l;
	Effect::RegistLight(std::move(_Light));

	if (_AnimationTextures.GetAnimationKey() == L"Staff_Loop")
	{
		StaffChargeT += fDeltaTime;
		if (StaffChargeT >= 4.0f)
		{
			StaffChargeT = 4.0f;
		};

		MyLight _Light{};
		_Light.Location =MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 0,0,0.0f + (StaffChargeT *0.5f),1 };
		_Light.Priority = 1l;
		_Light.Radius =  (StaffChargeT * 50.0f);
		Effect::RegistLight(std::move(_Light));
	}

	auto iter = LightingDurationTable.find(L"SpellLight");
	if ( (iter != std::end(LightingDurationTable) ) && _AnimationTextures.GetAnimationKey() == L"Light"  &&  ( iter->second<=0.0f) )
	{
		auto* const _ScreenEffect = dynamic_cast<CScreenEffect* const> (m_pManagement->GetGameObject(-1, L"Layer_" + TYPE_NAME<CScreenEffect>(), 0));
		_ScreenEffect->Blur();

		MyLight _Light{};
		_Light.Location = MATH::ConvertVec4(m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.0f, 1.f);
		const vec4 Diffuse = MATH::ConvertVec4(vec3{ 2.f,2.f,2.f }, 1.f);
		_Light.Diffuse = Diffuse;
		_Light.Priority = 1l;
		_Light.Radius = SpellLightRadius;
		Effect::RegistLight(std::move(_Light));

		auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");
		Sphere LightHitSphere;
		LightHitSphere.Center = m_pTransformCom->GetLocation();
		LightHitSphere.Radius = SpellLightHitRadius;

		for (auto& _CurrentMonster : _MonsterList)
		{
			auto _Component = _CurrentMonster->GetComponent(CComponent::Tag + TYPE_NAME<CCollisionComponent >());
			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>IsCollision = 
					Collision::IsSphereToSphere(LightHitSphere, _CollisionComp->_Sphere);

				if (IsCollision.first)
				{
					dynamic_cast<CMonster* const>(_CollisionComp->Owner)->FlashHit();
				}
			}
		}
	};

	if (_AnimationTextures.GetAnimationKey() == L"Freeze")
	{
		FreezeParticlePush();
		auto* const _ScreenEffect = dynamic_cast<CScreenEffect* const> (m_pManagement->GetGameObject(-1, L"Layer_" + TYPE_NAME<CScreenEffect>(), 0));
		_ScreenEffect->FreezeEffect();

		auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");
		Sphere FreezeHitSphere;
		FreezeHitSphere.Center = m_pTransformCom->GetLocation();
		FreezeHitSphere.Radius = SpellFreezeHitRadius;

		for (auto& _CurrentMonster : _MonsterList)
		{
			auto _Component = _CurrentMonster->GetComponent(CComponent::Tag + TYPE_NAME<CCollisionComponent >());
			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>IsCollision =
					Collision::IsSphereToSphere(FreezeHitSphere, _CollisionComp->_Sphere);

				if (IsCollision.first)
				{
					this->CurrentAttack = 20.f * _DeltaTime;
					dynamic_cast<CMonster* const>(_CollisionComp->Owner)->FreezeHit();
				}
			}
		}
	};


	T += fDeltaTime;

	return _uint();
}

_uint CPlayer::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	bWeaponEffectRender = false;

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	if (PrevLocation!= m_pTransformCom->GetLocation())
	{
		T += fDeltaTime * 7.f;
	}
	else
	{
		T =0.0f;
	};

	if (T < 0.0f)T = 0.0f;

	PrevLocation = m_pTransformCom->GetLocation();

	if (_AnimationTextures.GetAnimationKey() == L"Flak_Fire")
	{
		LightingDurationTable[L"FlakFire"] = 0.1f;
		bWeaponEffectRender = true;
	}

	if (_AnimationTextures.GetAnimationKey() == L"ElectricStaff_Fire")
	{
		CurrentElectricStaffFireDamage += fDeltaTime * ElectricStaffReinForceTimeRequired;
		CurrentElectricStaffFireDamage = boost::algorithm::clamp<float>(CurrentElectricStaffFireDamage, 0.0f, ElectricStaffDamageLimitTable[2]);
		LightingDurationTable[L"ElectricStaffFire"] = 0.1f;
		bWeaponEffectRender = true;
	}
	else
	{
		CurrentElectricStaffFireDamage = 1.f;
	}

	auto* const _ScreenEffect = dynamic_cast<CScreenEffect* const> (m_pManagement->GetGameObject(-1, L"Layer_" + TYPE_NAME<CScreenEffect>(), 0));
	_ScreenEffect->Shield(CalcShieldStep());

	return _uint();
}

HRESULT CPlayer::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	float X = std::cosf(T) * 20.f;
	float Y = std::sinf(T) * 40.f;

	mat PrevView, PrevProjection , ViewIdentity, Ortho;

	m_pDevice->GetTransform(D3DTS_VIEW, &PrevView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &PrevProjection);

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	D3DXMatrixOrthoLH(&Ortho, WINCX, WINCY, 0.0f, 
		FLT_MAX);

	const auto& TextureTuple = _AnimationTextures.GetCurrentTexture();

	D3DSURFACE_DESC _Desc;
	std::get<0>(TextureTuple)->GetLevelDesc(0, &_Desc);

	const float 	Bottom =      -(WINCY / 2.f);
	const float XSize = (float)_Desc.Width * 4.f;
	const float YSize = (float)_Desc.Height * 4.f;
	mat GunUI = MATH::WorldMatrix({ XSize,YSize,0 }, { 0,0,0 }, 
		{ 0+X,Bottom+ (YSize /2.f) + Y -40.f ,
		0.0f });
	D3DXMatrixIdentity(&ViewIdentity);

	_Effect.SetVSConstantData(m_pDevice, "bUI", 1l);
	_Effect.SetVSConstantData(m_pDevice, "World", GunUI);
	_Effect.SetVSConstantData(m_pDevice, "View", ViewIdentity);
	_Effect.SetVSConstantData(m_pDevice, "Projection", Ortho);

	m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),std::get<0> (TextureTuple));
	/*m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"), std::get<1>(TextureTuple));
	m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"), std::get<2>(TextureTuple));*/
	
	_Effect.SetPSConstantData(m_pDevice, "bUI", 1l);
	_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0 );
	_Effect.SetPSConstantData(m_pDevice,"bNormalSamplerBind", 0);
	_Effect.SetPSConstantData(m_pDevice, "Shine", 20.f);
	
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);

	_VertexBuffer->Render();
	/*if (bWeaponEffectRender)
		WeaponEffectOrthoRender(_Effect);*/

	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);
	_Effect.SetPSConstantData(m_pDevice, "bUI", 0l);
	_Effect.SetVSConstantData(m_pDevice, "View", PrevView);
	_Effect.SetVSConstantData(m_pDevice, "Projection", PrevProjection);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_CollisionComp->DebugDraw();

	return S_OK;
}

void CPlayer::Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo)
{
	if (_Target->CurrentAttack > 0.0f)
	{
		if (RemainShield > 0.0f)
		{
			RemainShield -= _Target->CurrentAttack;
		}
		else
		{
			_CurrentInfo.HP -= _Target->CurrentAttack;

			auto* const _ScreenEffect = dynamic_cast<CScreenEffect* const> (m_pManagement->GetGameObject(-1, L"Layer_" + TYPE_NAME<CScreenEffect>(), 0));
			_ScreenEffect->BloodEffect();
			auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
			_Camera->Shake(_Target->CurrentAttack / 15.0f, MATH::RandVec(), _Target->CurrentAttack / 15.0f);
		}
		return;
	}

	auto* _Item = dynamic_cast<CItem*>(_Target);
	if (_Item)
	{
		auto *const _ScreenEffect = dynamic_cast<CScreenEffect* const > (m_pManagement->GetGameObject(-1, L"Layer_" + TYPE_NAME<CScreenEffect>(), 0));
		_ScreenEffect->ItemInteractionEffect();
		_ScreenEffect->BloodEffect();
		switch (_Item->GetItemInfo().etype)
		{
		case Item::HealthBig:
			_CurrentInfo.HP += 10.f;
			break;
		case Item::HealthSmall:
			_CurrentInfo.HP += 5.f;
			break;
		case Item::ManaBig:
			_CurrentInfo.MP += 10.f ;
			break;
		case Item::ManaSmall:
			_CurrentInfo.MP += 5.f;
			break;
		case Item::Ammo:
			_CurrentInfo.Ammo += 20l;
			break;
		case Item::KeyBlue:
			bKeyBlue = true;
			break;
		case Item::KeyRed:
			bKeyRed= true;
			break;
		case Item::KeyYellow:
			bKeyYellow = true;
			break;
		case Item::Upgrade:
			bUpgrade = true;
			break;
		default:
			break;
		}
	};
}

void CPlayer::MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo)
{
	if (_CollisionInfo.Flag == L"Floor")
	{
		int i = 0;
	}
	else if (_CollisionInfo.Flag == L"Wall")
	{
		int i = 0;
	}
}

void CPlayer::MoveForward(const float DeltaTime)&
{
	PlayStepSound();

	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Forward{ world._31,0.f,world._33 };
	Forward = MATH::Normalize(Forward);
	float Speed = Desc.fSpeedPerSec;
	if (_CurrentWeaponState == EWeaponState::ElectricStaff)
	{
		Speed *= 0.5f;
	}
	Desc.vPosition += Forward * Speed * DeltaTime;
};
void CPlayer::MoveRight(const float DeltaTime)&
{
	PlayStepSound();
	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Right{ world._11,0.f,world._13 };
	Right = MATH::Normalize(Right);
	float Speed = Desc.fSpeedPerSec;
	if (_CurrentWeaponState == EWeaponState::ElectricStaff)
	{
		Speed *= 0.5f;
	}
	Desc.vPosition += Right * Speed * DeltaTime;
}
void CPlayer::MouseRight()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		DaggerThrow();
		break;
	case CPlayer::EWeaponState::ShotGun:
		break;
	case CPlayer::EWeaponState::Akimbo:
		break;
	case CPlayer::EWeaponState::Magnum:
		break;
	case CPlayer::EWeaponState::Staff:
		break;
	default:
		break;
	}
}
void CPlayer::MouseRightPressing()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		break;
	case CPlayer::EWeaponState::ShotGun:
		break;
	case CPlayer::EWeaponState::Akimbo:
		break;
	case CPlayer::EWeaponState::Magnum:
		break;
	case CPlayer::EWeaponState::Staff:
		if (_AnimationTextures.GetAnimationKey() != L"Staff_Charge" && 
			_AnimationTextures.GetAnimationKey() != L"Staff_Loop")
		{
			StaffCharge();

		}
		break;
	default:
		break;
	}	
}
void CPlayer::MouseRightUp()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		break;
	case CPlayer::EWeaponState::ShotGun:
		break;
	case CPlayer::EWeaponState::Akimbo:
		break;
	case CPlayer::EWeaponState::Magnum:
		break;
	case CPlayer::EWeaponState::Staff:
		if (bStaffLoop)
		{
			bStaffLoop = false;
			StaffRelease();
		}
		break;
	default:
		break;
	}
};
void CPlayer::MouseLeft()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		DaggerStab();
		break;
	case CPlayer::EWeaponState::ShotGun:
		ShotGunShot();
		break;
	case CPlayer::EWeaponState::Akimbo:
		AkimboFire();
		break;
	case CPlayer::EWeaponState::Magnum:
		MagnumFire();
		break;
	case CPlayer::EWeaponState::Staff:
		if (bStaffLoop)
		{
			bStaffLoop = false;
			StaffRelease();
		}
		else
		{
			StaffFire();
		}
		break;
	case CPlayer::EWeaponState::Dynamite:
		DynamiteThrow();
		break;
	default:
		break;
	}
}
void CPlayer::MouseLeftPressing()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		break;
	case CPlayer::EWeaponState::ShotGun:
		break;
	case CPlayer::EWeaponState::Akimbo:
		if (_AnimationTextures.GetAnimationKey() != L"Akimbo_Fire")
		{
			AkimboFire();
		}
		break;
	case CPlayer::EWeaponState::Magnum:
		break;
	case CPlayer::EWeaponState::Staff:
		break;
	case CPlayer::EWeaponState::ElectricStaff:
			ElectricStaffFire();
		break;
	case CPlayer::EWeaponState::Flak:
			FlakFire();
		break;
	default:
		break;
	};
}
void CPlayer::MouseLeftUp()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::ElectricStaff:
		if (_AnimationTextures.GetAnimationKey() == L"ElectricStaff_Fire")
		{
			_AnimationTextures.ChangeAnim(L"ElectricStaff_Idle", WeaponAnimDelta, 1ul, true);
		};
		break;
	case CPlayer::EWeaponState::Flak:
		if (_AnimationTextures.GetAnimationKey() == L"Flak_Fire")
		{
			_AnimationTextures.ChangeAnim(L"Flak_Idle", WeaponAnimDelta, 1ul, true);
		};
		break;
	default:
		break;
	}
}
;
void CPlayer::RButtonEvent()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		break;
	case CPlayer::EWeaponState::ShotGun:
		ShotGunReload();
		break;
	case CPlayer::EWeaponState::Flak:
		FlakReload();
		break;
	case CPlayer::EWeaponState::Akimbo:
		break;
	case CPlayer::EWeaponState::Magnum:
		break;
	case CPlayer::EWeaponState::Staff:
		break;
	default:
		break;
	}
}
void CPlayer::_1ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Dagger;
	_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX, 1);

}
void CPlayer::_2ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::ShotGun;
	_AnimationTextures.ChangeAnim(L"ShotGun_Idle", FLT_MAX, 1);
}
void CPlayer::_3ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Akimbo;
	_AnimationTextures.ChangeAnim(L"Akimbo_Idle", FLT_MAX, 1);
}
void CPlayer::_4ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Magnum;
	_AnimationTextures.ChangeAnim(L"Magnum_Idle", FLT_MAX, 1);
}
void CPlayer::_5ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Staff;
	_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1);
}

void CPlayer::_6ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Dynamite;
	_AnimationTextures.ChangeAnim(L"Dynamite_Idle", FLT_MAX, 1);
}

void CPlayer::_7ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::ElectricStaff;
	_AnimationTextures.ChangeAnim(L"ElectricStaff_Idle", FLT_MAX, 1);
}

void CPlayer::_8ButtonEvent()&
{
	_CurrentWeaponState = EWeaponState::Flak;
	_AnimationTextures.ChangeAnim(L"Flak_Idle", FLT_MAX, 1);
}

HRESULT CPlayer::AddStaticComponents()
{
	Super::AddStaticComponents();

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Player;
	_Info.bMapBlock= true;
	_Info.Owner = this;

	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Player");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pClone = new CPlayer(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	_AnimationTextures.AddRef();
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone Player");
		SafeRelease(pClone);
	}

	return pClone;
}

void CPlayer::Free()
{
	SafeRelease(_CollisionComp);
	_AnimationTextures.Release();
	SafeRelease(_VertexBuffer);

	CGameObject::Free();
}

static auto  PlayBulletHitParticle = [](const vec3 Dir, const vec3 IntersectPoint)
{
	Particle _Particle;
	_Particle.bBillboard = true;
	_Particle.Delta = 0.07f;
	_Particle.Durtaion = _Particle.Delta * 4.0f;
	_Particle.EndFrame = 4ul;
	_Particle.Location = IntersectPoint + (Dir * 2.f);
	_Particle.Name = L"BulletHit" + std::to_wstring(MATH::RandInt({ 0,3 }));
	_Particle.Scale = { 0.7f,0.7f,0.7f };
	ParticleSystem::Instance().PushParticle(_Particle);
};
static auto PlaneEffect = [](CPlayer& _Player, const vec3 IntersectPoint, vec3 Normal ,float Scale)
{
	Particle _Particle;
	Normal = MATH::Normalize(Normal);
	_Particle.StartLocation = IntersectPoint + (Normal * 0.0001f);
	_Particle.Location = IntersectPoint + (Normal * 0.0001f);
	_Particle.Delta = FLT_MAX;
	_Particle.Durtaion = 180.0f;
	_Particle.EndFrame = 1ul;
	_Particle.Name = L"BulletHole" + std::to_wstring(MATH::RandInt({ 0,3 }));
	_Particle.Scale = { Scale,Scale,Scale };
	
	const vec3 Axis = MATH::Cross({ 0,0,-1 }, Normal);
	float Degree = MATH::ToDegree(std::acosf(MATH::Dot(Normal, vec3{ 0,0,-1 })));
	//if (MATH::almost_equal(Axis.y, 1.f) || MATH::almost_equal(Axis.y, 0.0f))
	//{
	//	Degree = 0.f;
	//};

	mat Rot = MATH::RotationMatrixFromAxis(Axis, Degree);
	_Particle.bRotationMatrix = true;
	_Particle.RotationMatrix = Rot;
	_Particle.bBillboard = false;
	ParticleSystem::Instance().PushParticle(std::move(_Particle));

	PlayBulletHitParticle(Normal, IntersectPoint);
};

void CPlayer::ShotGunShot()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"shotgun_shot.wav", CSoundMgr::PLAYER_WEAPON);
	AnimationTextures::NotifyType _Notify;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(1.2f, MATH::RandVec(), 0.15f);

	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);
	//	_Ray.Start = m_pTransformCom->GetLocation();

	_Notify[1ul] = [this, _Ray]()
	{

		{
			const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

			std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

			for (const auto& _CurPlane : _CurMap)
			{
				vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
				Normal = MATH::Normalize(Normal);
				if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
				float t = 0;
				vec3 IntersectPoint{ 0,0,0 };

				if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
				{
					const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
					_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
				}
			}

			auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
				const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
					return std::get<2>(_Lhs) < std::get<2>(_Rhs);
				});

			if (find_iter != std::end(_IntersectInfo))
			{
				for (size_t i = 0; i < 12; ++i)
				{
					vec3 Point = std::get<0>(*find_iter);
					Point += MATH::RandVec() * MATH::RandReal({ 1,2 });

					PlaneEffect(*this, Point, std::get<1>(*find_iter), 0.9f);
				};
			}
		}

		{
			const auto& _CurMap = CCollisionComponent::_MapFloorInfo;

			std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

			for (const auto& _CurPlane : _CurMap)
			{
				vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
				Normal = MATH::Normalize(Normal);
				if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
				float t = 0;
				vec3 IntersectPoint{ 0,0,0 };

				if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
				{
					const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
					_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
				}
			}

			auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
				const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
					return std::get<2>(_Lhs) < std::get<2>(_Rhs);
				});

			if (find_iter != std::end(_IntersectInfo))
			{

				for (size_t i = 0; i < 12; ++i)
				{
					vec3 Point = std::get<0>(*find_iter);
					Point += MATH::RandVec() * MATH::RandReal({ 1,2 });


					PlaneEffect(*this, Point, std::get<1>(*find_iter), 0.9f);
				};

			}
		}
	};

	_Notify[2ul] = [this, _Ray]()
	{
		ShotGunReload();
	};

	_AnimationTextures.ChangeAnim(L"ShotGun_Shot", 0.1f, 2ul,
		false, std::move(_Notify));

	this->CurrentAttack = 20.f;
	//  IntersectPoint
	std::vector< std::tuple<CGameObject*, const float, Collision::Info, vec3>  > _CollisionInfos;

	auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			vec3 IntersectPoint;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1, IntersectPoint);

			if (IsCollision.first)
			{
				_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3>
				{ _CurrentMonster, t0, IsCollision.second, IntersectPoint });
			}
		}
	}

	auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
		[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
			const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
				return std::get<1>(Lhs) < std::get<1>(Rhs);
		});

	if (find_iter != std::end(_CollisionInfos))
	{
		CGameObject* _Target = std::get<0>(*find_iter);
		const vec3 IntersectPoint = std::get<3>(*find_iter);
		const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
		_Target->Hit(this, (std::get<2>(*find_iter)));

		for (size_t i = 0; i < 8; ++i)
		{
			const vec3 RandPoint = IntersectPoint + MATH::RandVec() * MATH::RandReal({ 0.25,1 });
			PlayBulletHitParticle(Dir, RandPoint);
		};
	};

	_CollisionInfos.clear();

	{
		auto _DecoratorList = m_pManagement->GetGameObjects(-1, L"Layer_Decorator");

		for (auto& _CurrentDecorator : _DecoratorList)
		{
			auto _Component = _CurrentDecorator->GetComponent
			(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>
					IsCollision = Collision::IsRayToSphere(_Ray,
						_CollisionComp->_Sphere, t0, t1, IntersectPoint);

				if (IsCollision.first)
				{
					_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3 >{ _CurrentDecorator, t0, IsCollision.second, IntersectPoint});
				}
			}
		}

		auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
			[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
				const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
					return std::get<1>(Lhs) < std::get<1>(Rhs);
			});


		if (find_iter != std::end(_CollisionInfos))
		{
			const vec3 IntersectPoint = std::get<3>(*find_iter);
			CGameObject* _Target = std::get<0>(*find_iter);
			_Target->Hit(this, (std::get<2>(*find_iter)));
			const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);

			for (size_t i = 0; i < 8; ++i)
			{
				const vec3 RandPoint = IntersectPoint + MATH::RandVec() * MATH::RandReal({ 0.25,1 });
				PlayBulletHitParticle(Dir, RandPoint);
			};
		};

		_CollisionInfos.clear();
	}



	LightingDurationTable[L"ShotGunShot"] = 0.3f;
}

void CPlayer::ShotGunReload()
{

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"shotgun_pumpin.wav", CSoundMgr::PLAYER_WEAPON);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TANP);
	CSoundMgr::Get_Instance()->PlaySound(L"TanP.wav", CSoundMgr::TANP);

	AnimationTextures::NotifyType _Notify;

	_Notify[3ul] = [this]()
	{
		POINT _Pt;
		GetCursorPos(&_Pt);
		ScreenToClient(g_hWnd, &_Pt);
		vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
		Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);
	//	_Ray.Start = m_pTransformCom->GetLocation();

		CollisionParticle _CollisionParticle;
		_CollisionParticle.Delta = 10000.f;
		_CollisionParticle.bCollision = true;
		_CollisionParticle.bFloorCollision = true;
		_CollisionParticle.bWallCollision = false;
		_CollisionParticle.bMapBlock = true;
		_CollisionParticle.Gravity = 5.f;
		_CollisionParticle.bMove = true;

		vec3 SpawnLocation = (m_pTransformCom->GetLocation() + _Ray.Direction * 1.4f);

		if (MATH::RandInt({ 0,1 }))
		{
			SpawnLocation.x -= 0.1f;
		}
		else
		{
			SpawnLocation.x += 0.1f;
		}

		SpawnLocation.y -= 0.3f;
		_CollisionParticle.Scale = { 0.43f,0.43f,0.43f };
		_CollisionParticle.StartLocation = SpawnLocation;
		_CollisionParticle.Location = SpawnLocation;
		_CollisionParticle.Dir = vec3{ MATH::RandReal({-1,1}),0.f,MATH::RandReal({-1,1}) };
		_CollisionParticle.Angle = MATH::RandReal({ 90,130 });
		_CollisionParticle.Speed = MATH::RandReal({ 50,150 });
		_CollisionParticle.Rotation = { 0.f,0.f,MATH::RandReal({-360,360}) };
		_CollisionParticle.Durtaion = 180.0f;
		_CollisionParticle.Name = L"ShotGunShell";
		_CollisionParticle.Radius = 0.3f;
		_CollisionParticle.Speed = 10.f;
		ParticleSystem::Instance().PushCollisionParticle
		(std::move(_CollisionParticle));
	};
	_Notify[23ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"ShotGun_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"ShotGun_Reload", 0.07f, 14ul ,
		false, std::move(_Notify));
}

void CPlayer::DaggerStab()
{
	int iRandomIndex = rand() % 2 + 1;
	TCHAR szSoundKey[64] = L"";
	swprintf_s(szSoundKey, L"dagger_metal_swing_%d.wav", iRandomIndex);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(szSoundKey, CSoundMgr::PLAYER_WEAPON);
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX,1);
	};

	_AnimationTextures.ChangeAnim(L"Dagger_Stab", 0.07f, 4ul, false,
		std::move(_Notify));

	Sphere _Sphere;
	const float DaggerRich = 1.f;
	const float DaggerRange = 2.f;
	_Sphere.Center = m_pTransformCom->GetLocation() + (m_pTransformCom->GetLook() * DaggerRich);
	_Sphere.Radius = DaggerRange;

	this->CurrentAttack = 10.f;
	auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			vec3 IntersectPoint;

			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsSphereToSphere(_Sphere,_CollisionComp->_Sphere);

			if (IsCollision.first)
			{
				Collision::Info _CollisionInfo = IsCollision.second;
				_CurrentMonster->Hit(this, std::move(_CollisionInfo));
			}
		}
	}

	{
		auto _DecoratorList = m_pManagement->GetGameObjects(-1, L"Layer_Decorator");

		for (auto& _CurrentDecorator : _DecoratorList)
		{
			auto _Component = _CurrentDecorator->GetComponent
			(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;

				std::pair<bool, Engine::Collision::Info>
					IsCollision = Collision::IsSphereToSphere(_Sphere, _CollisionComp->_Sphere);

				if (IsCollision.first)
				{
					Collision::Info _CollisionInfo = IsCollision.second;
					_CurrentDecorator->Hit(this, std::move(_CollisionInfo));
				}
			}
		}
	};
}

void CPlayer::DaggerThrow()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"knife_throw.wav", CSoundMgr::PLAYER_WEAPON);

	AnimationTextures::NotifyType _Notify;

	_Notify[12ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Dagger_Throw", 0.07f, 12ul, false,
		std::move(_Notify));

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.30f, MATH::RandVec(), 0.30f );

	for (size_t i = 0; i < 1; ++i)
	{
		CollisionParticle _Particle;
		_Particle.bBillboard = false;
		_Particle.Delta = 2;
		_Particle.Durtaion = 10.0f;
		_Particle.EndFrame = 1;
		m_pTransformCom->GetRight() * 0.5f;
		 
		_Particle.Location = m_pTransformCom->GetLocation()+ m_pTransformCom->GetRight() * 0.25f + m_pTransformCom->GetUp() *0.25f;
		const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
		_Particle.Dir = MATH::Normalize(WorldGoal - _Particle.Location);
		_Particle.UVAlphaLerp = 1l;
		_Particle.bRotationMatrix = true;
		_Particle.bWallCollision = true;
		_Particle.bFloorCollision = true;
		_Particle.bCollision = true;
		_Particle._Tag = CCollisionComponent::ETag::PlayerAttackParticle;
		_Particle.CurrentAttack = 15.f;

		const float AngleY = -89.f;
		const float AngleZ = -40.f;
		vec3 StandardNormal = { 0,0,-1 };
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,1,0 }, AngleY);
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,0,1 }, AngleZ);
		mat RotY, RotZ;
		D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));
		D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(AngleZ));

		vec3 CameraLook = { 0,0,1 };
		vec3 Dir = MATH::Normalize(_Particle.Dir);
		vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, Dir ));
		float Angle = std::acosf(MATH::Dot(Dir , CameraLook ));
		mat RotAxis;
		D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
		mat Rot = RotY * RotZ * RotAxis;
		/*D3DXMatrixRotationAxis(&RotAxis, &Dir, MATH::ToRadian(Angle));
		Rot *= RotAxis;*/
		
		_Particle.RotationMatrix = Rot;
		_Particle.Radius = 1.f;

		_Particle.Scale = { 2.f,0.5f,0.f };
		_Particle.Name = L"DaggerThrow";
		_Particle.LightCalcFlag = 1l;
		_Particle.Speed = 40.f;

		ParticleSystem::Instance().PushCollisionParticle(_Particle);

		for (size_t i = 0; i < 2; ++i)
		{
			CollisionParticle _ArrowParticle = _Particle;
			_ArrowParticle.Speed *= 2.0f;
			_ArrowParticle.Scale.y = 2.0f;
			_ArrowParticle.Scale.x = -20.f;
			_ArrowParticle.Name = L"ArrowX";
			_ArrowParticle.bWallCollision = false;
			_ArrowParticle.bFloorCollision = false;
			_ArrowParticle.bCollision = false;
			_ArrowParticle.Location += _ArrowParticle.Dir * _Particle.Scale.x;
			_ArrowParticle.	UVAlphaLerp = 0l;
			_ArrowParticle.LightCalcFlag = 1l;
			mat RotDirAxis;
			D3DXMatrixRotationAxis(&RotDirAxis, &_ArrowParticle.Dir, MATH::ToRadian(90.f* i));
			_ArrowParticle.RotationMatrix *= RotDirAxis;
			ParticleSystem::Instance().PushCollisionParticle(_ArrowParticle);
		};
	}
}

void CPlayer::AkimboFire()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"pistol_shot.wav", CSoundMgr::PLAYER_WEAPON);
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Akimbo_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Akimbo_Fire", 0.025f, 4ul, false, std::move(_Notify));

	LightingDurationTable[L"AkimboFire"] = 0.2f;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.35f, MATH::RandVec(), 0.07f);

	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);

	this->CurrentAttack = 1.5f;
	                                                         //  IntersectPoint
	std::vector< std::tuple<CGameObject*, const float, Collision::Info,vec3>  > _CollisionInfos;

	auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			vec3 IntersectPoint;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1, IntersectPoint);

			if (IsCollision.first)
			{
				_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3>{ _CurrentMonster, t0, IsCollision.second ,IntersectPoint });
			}
		}
	}

	auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
		[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
			const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
				return std::get<1>(Lhs) < std::get<1>(Rhs);
		});

	if (find_iter != std::end(_CollisionInfos))
	{
		CGameObject* _Target = std::get<0>(*find_iter);
		const vec3 IntersectPoint = std::get<3>(*find_iter); 
		const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
		PlayBulletHitParticle(Dir, IntersectPoint);
		_Target->Hit(this, (std::get<2>(*find_iter)));
	};

	_CollisionInfos.clear();

	{
		auto _DecoratorList = m_pManagement->GetGameObjects(-1, L"Layer_Decorator");

		for (auto& _CurrentDecorator : _DecoratorList)
		{
			auto _Component = _CurrentDecorator->GetComponent
			(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>
					IsCollision = Collision::IsRayToSphere(_Ray,
						_CollisionComp->_Sphere, t0, t1, IntersectPoint);

				if (IsCollision.first)
				{
					_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info , vec3 >{ _CurrentDecorator, t0, IsCollision.second , IntersectPoint});
				}
			}
		}

		auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
			[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
				const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
					return std::get<1>(Lhs) < std::get<1>(Rhs);
			});


		if (find_iter != std::end(_CollisionInfos))
		{
			CGameObject* _Target = std::get<0>(*find_iter);
			const vec3 IntersectPoint = std::get<3>(*find_iter);
			const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
			PlayBulletHitParticle(Dir, IntersectPoint);
			_Target->Hit(this, (std::get<2>(*find_iter)));
		};

		_CollisionInfos.clear();
	}




	CollisionParticle _CollisionParticle;
	_CollisionParticle.Delta = 10000.f;
	_CollisionParticle.bCollision = true;
	_CollisionParticle.bFloorCollision = true;
	_CollisionParticle.bWallCollision = false;
	_CollisionParticle.bMapBlock = true;
	_CollisionParticle.Gravity = 5.f;
	vec3 SpawnLocation = (m_pTransformCom->GetLocation() + _Ray.Direction* 1.4f);

	if (MATH::RandInt({ 0,1 }))
	{
		SpawnLocation.x -= 0.1f;
	}
	else
	{
		SpawnLocation.x += 0.1f;
	}

	SpawnLocation.y -= 0.3f;
	_CollisionParticle.Scale = { 0.43f,0.43f,0.43f };
	_CollisionParticle.StartLocation = SpawnLocation;
	_CollisionParticle.Location = SpawnLocation;
	_CollisionParticle.Dir = vec3{ MATH::RandReal({-1,1}),0.f,MATH::RandReal({-1,1}) };
	_CollisionParticle.Angle = MATH::RandReal({ 90,130});
	_CollisionParticle.Speed = MATH::RandReal({ 50,150});
	_CollisionParticle.Rotation = { 0.f,0.f,MATH::RandReal({-180,180}) };
	_CollisionParticle.Durtaion = 180.0f;
	_CollisionParticle.Name = L"BulletShell";
	_CollisionParticle.Radius = 0.3f;
	_CollisionParticle.Speed = 10.f;
	ParticleSystem::Instance().PushCollisionParticle(std::move(_CollisionParticle));

	{
		const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

		std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

		for (const auto& _CurPlane : _CurMap)
		{
			vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
			Normal = MATH::Normalize(Normal);
			if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
			float t = 0;
			vec3 IntersectPoint{ 0,0,0 };

			if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
			{
				const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
				_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
			}
		}

		auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
			const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
				return std::get<2>(_Lhs) < std::get<2>(_Rhs);
			});

		if (find_iter != std::end(_IntersectInfo))
		{
		
			PlaneEffect(*this, std::get<0>(*find_iter), std::get<1>(*find_iter), 0.7f);
		}
	}

	{
		const auto& _CurMap = CCollisionComponent::_MapFloorInfo;

		std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

		for (const auto& _CurPlane : _CurMap)
		{
			vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
			Normal = MATH::Normalize(Normal);
			if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
			float t = 0;
			vec3 IntersectPoint{ 0,0,0 };

			if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
			{
				const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
				_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
			}
		}

		auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
			const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
				return std::get<2>(_Lhs) < std::get<2>(_Rhs);
			});

		if (find_iter != std::end(_IntersectInfo))
		{
			
			PlaneEffect(*this, std::get<0>(*find_iter), std::get<1>(*find_iter), 0.7f);
		}
	}
}

void CPlayer::MagnumFire()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"harvester_shot.wav", CSoundMgr::PLAYER_WEAPON);

	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Magnum_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Magnum_Fire", 0.07f, 4ul, false, std::move(_Notify));
	LightingDurationTable[L"MagnumFire"] = 0.2f;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.7f, MATH::RandVec(), 0.15f);

	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);

	this->CurrentAttack = 15.f;
	//  IntersectPoint
	std::vector< std::tuple<CGameObject*, const float, Collision::Info, vec3>  > _CollisionInfos;

	auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			vec3 IntersectPoint;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1, IntersectPoint);

			if (IsCollision.first)
			{
				_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3>{ _CurrentMonster, t0, IsCollision.second, IntersectPoint });
			}
		}
	}

	auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
		[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
			const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
				return std::get<1>(Lhs) < std::get<1>(Rhs);
		});

	if (find_iter != std::end(_CollisionInfos))
	{
		CGameObject* _Target = std::get<0>(*find_iter);
		const vec3 IntersectPoint = std::get<3>(*find_iter);
		const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
		for (size_t i = 0; i < 3; ++i)
		{
			const vec3 RandPoint = IntersectPoint + MATH::RandVec() * MATH::RandReal({ 0.2,0.4f });
			PlayBulletHitParticle(Dir, RandPoint);
		}
		_Target->Hit(this, (std::get<2>(*find_iter)));
	};

	_CollisionInfos.clear();

	{
		auto _DecoratorList = m_pManagement->GetGameObjects(-1, L"Layer_Decorator");

		for (auto& _CurrentDecorator : _DecoratorList)
		{
			auto _Component = _CurrentDecorator->GetComponent
			(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>
					IsCollision = Collision::IsRayToSphere(_Ray,
						_CollisionComp->_Sphere, t0, t1, IntersectPoint);

				if (IsCollision.first)
				{
					_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3 >{ _CurrentDecorator, t0, IsCollision.second, IntersectPoint});
				}
			}
		}

		auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
			[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
				const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
					return std::get<1>(Lhs) < std::get<1>(Rhs);
			});


		if (find_iter != std::end(_CollisionInfos))
		{
			CGameObject* _Target = std::get<0>(*find_iter);
			const vec3 IntersectPoint = std::get<3>(*find_iter);
			const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
			for (size_t i = 0; i < 3; ++i)
			{
				const vec3 RandPoint = IntersectPoint + MATH::RandVec() * MATH::RandReal({ 0.2,0.4f });
				PlayBulletHitParticle(Dir, RandPoint);
			}
			_Target->Hit(this, (std::get<2>(*find_iter)));
		};

		_CollisionInfos.clear();
	}




	CollisionParticle _CollisionParticle;
	_CollisionParticle.Delta = 10000.f;
	_CollisionParticle.bCollision = true;
	_CollisionParticle.bFloorCollision = true;
	_CollisionParticle.bWallCollision = false;
	_CollisionParticle.bMapBlock = true;
	_CollisionParticle.Gravity = 5.f;
	vec3 SpawnLocation = (m_pTransformCom->GetLocation() + _Ray.Direction * 1.4f);

	if (MATH::RandInt({ 0,1 }))
	{
		SpawnLocation.x -= 0.1f;
	}
	else
	{
		SpawnLocation.x += 0.1f;
	}

	SpawnLocation.y -= 0.3f;
	_CollisionParticle.Scale = { 0.43f,0.43f,0.43f };
	_CollisionParticle.StartLocation = SpawnLocation;
	_CollisionParticle.Location = SpawnLocation;
	_CollisionParticle.Dir = vec3{ MATH::RandReal({-1,1}),0.f,MATH::RandReal({-1,1}) };
	_CollisionParticle.Angle = MATH::RandReal({ 90,130 });
	_CollisionParticle.Speed = MATH::RandReal({ 50,150 });
	_CollisionParticle.Rotation = { 0.f,0.f,MATH::RandReal({-360,360}) };
	_CollisionParticle.Durtaion = 180.0f;
	_CollisionParticle.Name = L"MagnumShell";
	_CollisionParticle.Radius = 0.3f;
	_CollisionParticle.Speed = 10.f;
	ParticleSystem::Instance().PushCollisionParticle
	(std::move(_CollisionParticle));
	

	{
		const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

		std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

		for (const auto& _CurPlane : _CurMap)
		{
			vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
			Normal = MATH::Normalize(Normal);
			if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
			float t = 0;
			vec3 IntersectPoint{ 0,0,0 };

			if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
			{
				const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
				_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
			}
		}

		auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
			const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
				return std::get<2>(_Lhs) < std::get<2>(_Rhs);
			});

		if (find_iter != std::end(_IntersectInfo))
		{
			
			PlaneEffect(*this, std::get<0>(*find_iter), std::get<1>(*find_iter), 1.25f);
		}
	}
	
	{
		const auto& _CurMap = CCollisionComponent::_MapFloorInfo;

		std::vector<std::tuple<vec3, vec3, float>> _IntersectInfo;

		for (const auto& _CurPlane : _CurMap)
		{
			vec3 Normal{ _CurPlane._Plane.a, _CurPlane._Plane.b, _CurPlane._Plane.c };
			Normal = MATH::Normalize(Normal);
			if (MATH::Dot(Normal, _Ray.Direction) > 0)continue;
			float t = 0;
			vec3 IntersectPoint{ 0,0,0 };

			if (Collision::IsTriangleToRay(_CurPlane, _Ray, t, IntersectPoint))
			{
				const vec3 Normal{ _CurPlane._Plane.a , _CurPlane._Plane.b, _CurPlane._Plane.c };
				_IntersectInfo.push_back({ IntersectPoint, Normal ,t });
			}
		}

		auto find_iter = std::min_element(std::begin(_IntersectInfo), std::end(_IntersectInfo), [](
			const std::tuple < vec3, vec3, float>& _Lhs, const std::tuple < vec3, vec3, float>& _Rhs) {
				return std::get<2>(_Lhs) < std::get<2>(_Rhs);
			});

		if (find_iter != std::end(_IntersectInfo))
		{
		
			PlaneEffect(*this, std::get<0>(*find_iter), std::get<1>(*find_iter), 1.25f);
		}
	}
}

void CPlayer::StaffFire()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"Staff_Shot.wav", CSoundMgr::PLAYER_WEAPON);
	AnimationTextures::NotifyType _Notify;
	bStaffLoop = false;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Staff_Fire", 0.07f, 4ul, false, std::move(_Notify));

	LightingDurationTable[L"StaffFire"] = 0.15f;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.25f, MATH::RandVec(), 0.25f);

	for (size_t i = 0; i < 1; ++i)
	{
		CollisionParticle _Particle;
		_Particle.bBillboard = false;
		_Particle.Delta = 2;
		_Particle.Durtaion = 10.0f;
		_Particle.EndFrame = 1;

		_Particle.Location = m_pTransformCom->GetLocation() +  (m_pTransformCom->GetUp() * -0.25f );
		const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
		_Particle.Dir = MATH::Normalize(WorldGoal - _Particle.Location);
		_Particle.UVAlphaLerp = 1l;
		_Particle.bRotationMatrix = true;
		_Particle.bWallCollision = true;
		_Particle.bFloorCollision = true;
		_Particle.bCollision = true;
		_Particle._Tag = CCollisionComponent::ETag::PlayerAttackParticle;
		_Particle.CurrentAttack = StaffAttack;
		_Particle.LightCalcFlag = 1l;

		const float AngleY = -90.f;
		const float AngleZ = -90.f;
		vec3 StandardNormal = { 0,0,-1 };
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,1,0 }, AngleY);
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,0,1 }, AngleZ);
		mat RotY, RotZ;
		D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));
		D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(AngleZ));

		vec3 CameraLook = { 0,0,1 };
		vec3 Dir = MATH::Normalize(_Particle.Dir);
		vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, Dir));
		float Angle = std::acosf(MATH::Dot(Dir, CameraLook));
		mat RotAxis;
		D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
		mat Rot = RotY * RotZ * RotAxis;
		

		_Particle.RotationMatrix = Rot;
		_Particle.Radius = 1.f;

		_Particle.Scale = { 2.f,0.5f,0.f };
		_Particle.Name = L"StaffFire";

		_Particle.Speed = 40.f;

		ParticleSystem::Instance().PushCollisionParticle(_Particle);

		{
			Particle _BlastParticle;
			_BlastParticle.bBillboard = true;
			_BlastParticle.Delta = 0.1f;
			_BlastParticle.Durtaion = 10.f;
			_BlastParticle.EndFrame = 7ul;

			_BlastParticle.Location = m_pTransformCom->GetLocation() + (m_pTransformCom->GetUp() * -0.25f);
			const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
			_BlastParticle.Dir = MATH::Normalize(WorldGoal - _BlastParticle.Location);
			_BlastParticle.UVAlphaLerp = 1l;
			_BlastParticle.LightCalcFlag = 1l;

			_BlastParticle.Scale = { 0.9f ,0.9f,0.9f };
			_BlastParticle.Name = L"WandProjectile";

			_BlastParticle.Speed = 100.0f;

			ParticleSystem::Instance().PushParticle(_BlastParticle);
		};

	}
}

void CPlayer::StaffCharge()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"staff_charge_loopable_sound_loop.wav", CSoundMgr::PLAYER_WEAPON);
	AnimationTextures::NotifyType _Notify;
	bStaffLoop = false;

	StaffChargeT = 0.0f;


	_Notify[16ul] = [this]()
	{
		StaffLoop();
	};

	_AnimationTextures.ChangeAnim(L"Staff_Charge", 0.07f, 16ul, false, std::move(_Notify));

	LightingDurationTable[L"StaffCharge"] = 0.0f;
}

void CPlayer::StaffRelease()
{
	AnimationTextures::NotifyType _Notify;

	bStaffLoop = false;

	_Notify[5ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1, true);
	};

	_AnimationTextures.ChangeAnim(L"Staff_Release", 0.07f, 5ul,
		false, std::move(_Notify));

	LightingDurationTable[L"StaffRelease"] = 0.2f;
	
	const float CurrentStaffAttack = (StaffChargeT * StaffAttack) + StaffAttack;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.25f * StaffChargeT, MATH::RandVec(), 0.4f);

	for (size_t i = 0; i < 1; ++i)
	{
		CollisionParticle _Particle;
		_Particle.bBillboard = false;
		_Particle.Delta = 2;
		_Particle.Durtaion = 10.0f;
		_Particle.EndFrame = 1;

		_Particle.Location = m_pTransformCom->GetLocation() + (m_pTransformCom->GetUp() * -0.25f);
		const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
		_Particle.Dir = MATH::Normalize(WorldGoal - _Particle.Location);
		_Particle.UVAlphaLerp = 1l;
		_Particle.bRotationMatrix = true;
		_Particle.bWallCollision = true;
		_Particle.bFloorCollision = true;
		_Particle.bCollision = true;
		_Particle._Tag = CCollisionComponent::ETag::PlayerAttackParticle;
		_Particle.CurrentAttack = CurrentStaffAttack;
		_Particle.LightCalcFlag = 1l;

		const float AngleY = -90.f;
		const float AngleZ = -90.f;
		vec3 StandardNormal = { 0,0,-1 };
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,1,0 }, AngleY);
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,0,1 }, AngleZ);
		mat RotY, RotZ;
		D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));
		D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(AngleZ));

		vec3 CameraLook = { 0,0,1 };
		vec3 Dir = MATH::Normalize(_Particle.Dir);
		vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, Dir));
		float Angle = std::acosf(MATH::Dot(Dir, CameraLook));
		mat RotAxis;
		D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
		mat Rot = RotY * RotZ * RotAxis;
		/*D3DXMatrixRotationAxis(&RotAxis, &Dir, MATH::ToRadian(Angle));
		Rot *= RotAxis;*/

		_Particle.RotationMatrix = Rot;
		_Particle.Radius = 1.f ;

		_Particle.Scale = { 3.f ,0.5f* StaffChargeT,0.f };
		_Particle.Name = L"StaffFire";

		_Particle.Speed = 40.f;

		ParticleSystem::Instance().PushCollisionParticle(_Particle);

		{
			Particle _BlastParticle;
			_BlastParticle.bBillboard = true;
			_BlastParticle.Delta = 0.1f;
			_BlastParticle.Durtaion = 10.f;
			_BlastParticle.EndFrame = 7ul;
			_BlastParticle.LightCalcFlag = 1l;

			_BlastParticle.Location = m_pTransformCom->GetLocation() + (m_pTransformCom->GetUp() * -0.25f);
			const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
			_BlastParticle.Dir = MATH::Normalize(WorldGoal - _BlastParticle.Location);
			_BlastParticle.UVAlphaLerp = 1l;

			_BlastParticle.Scale = { 0.65f * StaffChargeT,0.65f * StaffChargeT,0.65f * StaffChargeT };
			_BlastParticle.Name = L"WandProjectile";

			_BlastParticle.Speed = 100.0f;

			ParticleSystem::Instance().PushParticle(_BlastParticle);
		};
	}



	StaffChargeT = 0.0f;
}

void CPlayer::StaffLoop()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
	CSoundMgr::Get_Instance()->PlaySound(L"staff_charge_loopable_sound_loop.wav", CSoundMgr::PLAYER_WEAPON);
	_AnimationTextures.ChangeAnim(L"Staff_Loop", 0.07f, 10, true);
	// 공격 키 눌렀을때 웨폰 상태가 스태프라면 Fire 하고 난 다음에 
	// bStaffLoop =false 로 만들기
	// Key Up 일때 Loop 였다면 bStaffLoop = false 하고 Release 호출
	bStaffLoop = true;
}

void CPlayer::DynamiteThrow()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_WEAPON);
//	CSoundMgr::Get_Instance()->PlaySound(L"harvester_shot.wav", CSoundMgr::PLAYER_WEAPON);

	AnimationTextures::NotifyType _Notify;
	_Notify[9ul] = [this]()
	{
		auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
		const vec3 CameraLook = _Camera->GetTransform()->GetLook();
		
		{
			CollisionParticle _Particle;
			_Particle.bBillboard = true;
			_Particle.Delta = 2;
			_Particle.Durtaion = 3.0f;
			_Particle.Gravity = 1.5f;
			_Particle.EndFrame = 1;
			_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation() + m_pTransformCom->GetUp() * -0.25f +  (CameraLook * 2.f);

			const vec3 WorldGoal = _Camera->GetCameraDesc().vAt;
			{
				const vec3 CameraLook_X = MATH::Normalize({ 0.f,CameraLook.y,CameraLook.z });
				_Particle.Angle = 70.0f;
				_Particle.Dir = MATH::Normalize(WorldGoal - _Particle.Location);
				_Particle.Speed = 130.f;/* _Particle.Angle /45.f *10.f;*/
			}

			_Particle.bWallCollision = true;
			_Particle.bFloorCollision = true;
			_Particle.bCollision = true;
			_Particle._Tag = CCollisionComponent::ETag::PlayerAttackParticle;
			_Particle.CurrentAttack = 100.f;
			_Particle.bLoop = true;
			_Particle.Radius = 1.f;
			_Particle.Scale = { 0.5f,0.5f,0.5f };
			_Particle.Name = L"Dynamite";

			ParticleSystem::Instance().PushCollisionParticle(_Particle);
		}
	};

	_Notify[15ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Dynamite_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Dynamite_Throw", WeaponAnimDelta, 
		15ul, false, std::move(_Notify));


}

void CPlayer::FlakFire()
{
	if (_AnimationTextures.GetAnimationKey() != L"Flak_Fire")
	{
		_AnimationTextures.ChangeAnim(L"Flak_Fire", WeaponAnimDelta, 4ul,true);
	};
	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	_Camera->Shake(0.01f, MATH::RandVec(), 0.18f);
	LightingDurationTable[L"FlakFire"] = 0.1f;
}

void CPlayer::FlakReload()
{
	AnimationTextures::NotifyType _Notify;
	_Notify[15ul] = [this]() 
	{
		_AnimationTextures.ChangeAnim(L"Flak_Idle", FLT_MAX, 1ul);
	};

	_AnimationTextures.ChangeAnim(L"Flak_Reload", WeaponAnimDelta, 15ul, false ,std::move(_Notify));
}

void CPlayer::ElectricStaffFire()
{
	if (_AnimationTextures.GetAnimationKey() != L"ElectricStaff_Fire")
	{
		_AnimationTextures.ChangeAnim(L"ElectricStaff_Fire", WeaponAnimDelta, 6ul, true);
	}

	std::wstring EffectName;
	if (CurrentElectricStaffFireDamage <= ElectricStaffDamageLimitTable[0])
	{
		_WeaponEffectAnimTextures.ChangeAnim(L"Electric_Light", WeaponAnimDelta * 0.5f, 9ul, true);
		EffectName = L"ElectricLight";
	}
	else if (CurrentElectricStaffFireDamage <= ElectricStaffDamageLimitTable[1])
	{
		_WeaponEffectAnimTextures.ChangeAnim(L"Electric_Medium", WeaponAnimDelta * 0.5f, 9ul, true);
		EffectName = L"ElectricMedium";
	}
	else
	{
		_WeaponEffectAnimTextures.ChangeAnim(L"Electric_Heavy", WeaponAnimDelta * 0.5f, 9ul, true);
		EffectName = L"ElectricHeavy";
	}


	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	//_Camera->Shake(0.01f, MATH::RandVec(), CurrentElectricStaffFireDamage/1000.0f);
	LightingDurationTable[L"ElectricStaffFire"] = 0.1f;

	static auto CalcElectricInfo = [](Particle& _Particle, const vec3 Gizmo,
		const vec3 WorldGoal)
	{
		_Particle.Location = Gizmo;
		_Particle.Dir = MATH::Normalize(WorldGoal - _Particle.Location);
		_Particle.bRotationMatrix = true;
		const float AngleY = 90.0f;
		const float AngleZ = 90.0f;
		vec3 StandardNormal = { 0,0,-1 };
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,1,0 }, AngleY);
		StandardNormal = MATH::RotationVec(StandardNormal, { 0,0,1 }, AngleZ);
		mat RotY, RotZ;
		D3DXMatrixRotationY(&RotY, MATH::ToRadian(AngleY));
		D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(AngleZ));
		vec3 CameraLook = { 0,0,1 };
		vec3 Dir = MATH::Normalize(_Particle.Dir);
		vec3 Axis = MATH::Normalize(MATH::Cross(CameraLook, Dir));
		float Angle = std::acosf(MATH::Dot(Dir, CameraLook));
		mat RotAxis;
		D3DXMatrixRotationAxis(&RotAxis, &Axis, Angle);
		mat Rot = RotY * RotZ * RotAxis;
		_Particle.RotationMatrix = Rot;
	};

	boost::optional<Particle&> _Particle = ParticleSystem::Instance().GetParticle(EffectName);

	if (_Particle)
	{
		CalcElectricInfo(*_Particle, CalcElectricStaffGizmo(), _Camera->GetCameraDesc().vAt);
	}
	else
	{
		Particle _Particle;
		_Particle.bBillboard = false;
		_Particle.Delta = 0.05f;
		_Particle.UVAlphaLerp = 0l;
		_Particle.LightCalcFlag = 1l;
		_Particle.Scale = { 20.0f,0.7f,0.f };
		CalcElectricInfo(_Particle, CalcElectricStaffGizmo(), _Camera->GetCameraDesc().vAt);
		_Particle.Name = EffectName;
		_Particle.EndFrame = 9ul;
		_Particle.Durtaion = _Particle.Delta * _Particle.EndFrame;
		ParticleSystem::Instance().PushParticle(_Particle);
	}


	boost::optional<Particle&> _BlastParticle =
		ParticleSystem::Instance().GetParticle(L"ElectricBlast");

	static auto CalcBlastInfo = [](
		Particle& _Particle,
		const vec3 WorldGoal,
		const vec3 Gizmo,
		const float Rich)
	{
		const vec3 Dir = MATH::Normalize(WorldGoal - _Particle.Location);
		_Particle.Location = Gizmo + Dir * Rich;
	};

	if (_BlastParticle)
	{
		CalcBlastInfo(*_BlastParticle, _Camera->GetCameraDesc().vAt, CalcElectricStaffGizmo(), ElectricStaffFireRich);
	}
	else
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.bLoop = true;
		_Particle.bMove = false;
		_Particle.Delta = 0.03f;
		_Particle.EndFrame = 12ul;
		_Particle.Durtaion = _Particle.Delta * _Particle.EndFrame;
		_Particle.Name = L"ElectricBlast";
		_Particle.Scale = { 1.f,1.f,1.f };

		CalcBlastInfo(_Particle, _Camera->GetCameraDesc().vAt, CalcElectricStaffGizmo(), ElectricStaffFireRich);
		ParticleSystem::Instance().PushParticle(_Particle);
	}

	const vec3 Gizmo = CalcElectricStaffGizmo();
	const vec3 AttackDir = MATH::Normalize(_Camera->GetCameraDesc().vAt - Gizmo);
	const vec3 _ElectricParticleLocation = AttackDir * ElectricStaffFireRich + Gizmo;



	// 충돌 판정
	std::vector< std::tuple<CGameObject*, const float, Collision::Info, vec3>  > _CollisionInfos;

	auto _MonsterList = m_pManagement->GetGameObjects(-1, L"Layer_Monster");
	Ray _Ray;
	_Ray.Start = Gizmo;
	_Ray.Direction = AttackDir;
	this->CurrentAttack = CurrentElectricStaffFireDamage * _DeltaTime;

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

		auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			vec3 IntersectPoint;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1, IntersectPoint);

			if (IsCollision.first)
			{
				_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3>
				{ _CurrentMonster, t0, IsCollision.second, IntersectPoint });

			}
		}
	};

	auto find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
		[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
			const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
				return std::get<1>(Lhs) < std::get<1>(Rhs);
		});

	if (find_iter != std::end(_CollisionInfos))
	{
		CGameObject* _Target = std::get<0>(*find_iter);
		const vec3 IntersectPoint = std::get<3>(*find_iter);
		const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
		_Target->Hit(this, (std::get<2>(*find_iter)));

		for (size_t i = 0; i < 20; ++i)
		{
			Particle _Particle;
			_Particle.bBillboard = true;
			_Particle.Angle = MATH::RandReal({ 45,90 });
			_Particle.Dir = MATH::Normalize(MATH::RandVec());
			_Particle.bMove = true;
			_Particle.bLoop = false;
			_Particle.Delta = FLT_MAX;
			_Particle.Durtaion = 0.5f;
			_Particle.EndFrame = 1ul;
			_Particle.Gravity = MATH::RandReal({ 2.5,5.f });
			_Particle.LightCalcFlag = 1l;
			_Particle.Name = L"Particle";
			_Particle.Scale = { 0.025f,0.025f,0.025f };
			_Particle.Speed = MATH::RandReal({ 1.f,1.f * 3.f });
			_Particle.StartLocation = _Particle.Location = std::get<3>(*find_iter);
			ParticleSystem::Instance().PushParticle(_Particle);
		};

		return;
	};

	{
		auto _DecoratorList = m_pManagement->GetGameObjects(-1, L"Layer_Decorator");

		for (auto& _CurrentDecorator : _DecoratorList)
		{
			auto _Component = _CurrentDecorator->GetComponent
			(CComponent::Tag + TYPE_NAME<CCollisionComponent >());

			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
			if (_CollisionComp)
			{
				float t0 = 0;
				float t1 = 0;
				vec3 IntersectPoint;
				std::pair<bool, Engine::Collision::Info>
					IsCollision = Collision::IsRayToSphere(_Ray,
						_CollisionComp->_Sphere, t0, t1, IntersectPoint);

				if (IsCollision.first)
				{
					_CollisionInfos.emplace_back(std::tuple<CGameObject*, const float, Collision::Info, vec3>
					{ _CurrentDecorator, t0, IsCollision.second, IntersectPoint });
				}
			}
		}
	}

	 find_iter = std::min_element(std::begin(_CollisionInfos), std::end(_CollisionInfos),
		[](const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Lhs,
			const std::tuple<CGameObject*, const float, Collision::Info, vec3>& Rhs) {
				return std::get<1>(Lhs) < std::get<1>(Rhs);
		});

	if (find_iter != std::end(_CollisionInfos))
	{
		CGameObject* _Target = std::get<0>(*find_iter);
		const vec3 IntersectPoint = std::get<3>(*find_iter);
		const vec3 Dir = MATH::Normalize(m_pTransformCom->GetLocation() - IntersectPoint);
		_Target->Hit(this, (std::get<2>(*find_iter)));

		for (size_t i = 0; i < 20; ++i)
		{
			Particle _Particle;
			_Particle.bBillboard = true;
			_Particle.Angle = MATH::RandReal({ 45,90 });
			_Particle.Dir = MATH::Normalize(MATH::RandVec());
			_Particle.bMove = true;
			_Particle.bLoop = false;
			_Particle.Delta = FLT_MAX;
			_Particle.Durtaion = 0.5f;
			_Particle.EndFrame = 1ul;
			_Particle.Gravity = MATH::RandReal({ 2.5,5.f });
			_Particle.LightCalcFlag = 1l;
			_Particle.Name = L"Particle";
			_Particle.Scale = { 0.025f,0.025f,0.025f };
			_Particle.Speed = MATH::RandReal({ 1.f,1.f * 3.f });
			_Particle.StartLocation = _Particle.Location = std::get<3>(*find_iter);
			ParticleSystem::Instance().PushParticle(_Particle);
		};
	};
}

void CPlayer::FreezeParticlePush()&
{
	auto _Camera = dynamic_cast<CMainCamera* const>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	const vec3 Look = MATH::Normalize(_Camera->GetTransform()->GetLook());
	const vec3 Right = MATH::Normalize(_Camera->GetTransform()->GetRight());
	const vec3 ParticleScale = { 0.025f,0.025f,0.025f };
	const float Duration = 1.f;
	const std::pair<float,float> SpeedRange = { 6.f,12.f};
	const std::pair<float, float > GravityRange = { 10.f,20.f};
	const float OffSetScale = 4.f;

	for (size_t i = 0; i < 1000.f*_DeltaTime; ++i)
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.Angle = MATH::RandReal({ 45,90 });
		_Particle.Dir = MATH::Normalize(MATH::RandVec());
		_Particle.bMove = true;
		_Particle.bLoop = false;
		_Particle.Delta = FLT_MAX;
		_Particle.Durtaion = Duration;
		_Particle.EndFrame = 1ul;
		_Particle.Gravity = MATH::RandReal(GravityRange);
		_Particle.LightCalcFlag = 1l;
		_Particle.Name = L"Snow";
		_Particle.Scale = ParticleScale;
		_Particle.Speed = MATH::RandReal(SpeedRange);
		_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation()  + Look * OffSetScale;
		ParticleSystem::Instance().PushParticle(_Particle);
	};

	for (size_t i = 0; i < 5; ++i)
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.Angle = MATH::RandReal({ 45,90 });
		_Particle.Dir = MATH::Normalize(MATH::RandVec());
		_Particle.bMove = true;
		_Particle.bLoop = false;
		_Particle.Delta = FLT_MAX;
		_Particle.Durtaion = Duration;
		_Particle.EndFrame = 1ul;
		_Particle.Gravity = MATH::RandReal(GravityRange);
		_Particle.LightCalcFlag = 1l;
		_Particle.Name = L"Snow";
		_Particle.Scale = ParticleScale;
		_Particle.Speed = MATH::RandReal(SpeedRange);
		_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation() + -Look * OffSetScale;
		ParticleSystem::Instance().PushParticle(_Particle);
	};

	for (size_t i = 0; i < 5; ++i)
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.Angle = MATH::RandReal({ 45,90 });
		_Particle.Dir = MATH::Normalize(MATH::RandVec());
		_Particle.bMove = true;
		_Particle.bLoop = false;
		_Particle.Delta = FLT_MAX;
		_Particle.Durtaion = Duration;
		_Particle.EndFrame = 1ul;
		_Particle.Gravity = MATH::RandReal(GravityRange);
		_Particle.LightCalcFlag = 1l;
		_Particle.Name = L"Snow";
		_Particle.Scale = ParticleScale;
		_Particle.Speed = MATH::RandReal(SpeedRange);
		_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation() + Right * OffSetScale;
		ParticleSystem::Instance().PushParticle(_Particle);
	};

	for (size_t i = 0; i < 5; ++i)
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.Angle = MATH::RandReal({ 45,90 });
		_Particle.Dir = MATH::Normalize(MATH::RandVec());
		_Particle.bMove = true;
		_Particle.bLoop = false;
		_Particle.Delta = FLT_MAX;
		_Particle.Durtaion = Duration;
		_Particle.EndFrame = 1ul;
		_Particle.Gravity = MATH::RandReal(GravityRange);
		_Particle.LightCalcFlag = 1l;
		_Particle.Name = L"Snow";
		_Particle.Scale = ParticleScale;
		_Particle.Speed = MATH::RandReal(SpeedRange);
		_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation() + -Right * OffSetScale;
		ParticleSystem::Instance().PushParticle(_Particle);
	};
}

void CPlayer::SpellFreeze()
{
	AnimationTextures::NotifyType _AnimNotify;
	_AnimationTextures.ChangeAnim(L"Freeze", 0.08f, 11ul, true, std::move(_AnimNotify));
};

void CPlayer::SpellLight()
{
	AnimationTextures::NotifyType _AnimNotify;

	/*_AnimNotify [5ul]= [this ,CurAnimInfo = _AnimationTextures.GetAnimInfo()]()
	{
		_AnimationTextures.ChangeAnim(CurAnimInfo.CurrentAnimKey, CurAnimInfo.AnimDelta, CurAnimInfo.ImgNum, CurAnimInfo.bLoop, CurAnimInfo.CurrentAnimNotify, 0.0f, 0ul);
	};*/

	LightingDurationTable[L"SpellLight"] = SpellLightDuration;
	_AnimationTextures.ChangeAnim(L"Light", 0.08f, 5ul, true, std::move(_AnimNotify));
};

void CPlayer::WeaponEffectOrthoRender(Effect::Info& _Effect)
{
	DWORD ZWriteEnableFlag;
	m_pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &ZWriteEnableFlag);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);  
	float X = std::cosf(T) * 20.f;
	float Y = std::sinf(T) * 40.f;

	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));
	const auto& TextureTuple = _WeaponEffectAnimTextures.GetCurrentTexture();
	D3DSURFACE_DESC _Desc;
	std::get<0>(TextureTuple)->GetLevelDesc(0, &_Desc);

	const float 	Bottom = -(WINCY / 2.f);
	const float XSize = (float)_Desc.Width * 4.f;
	const float YSize = (float)_Desc.Height * 4.f;
	const float FloorY = Bottom + (YSize / 2.f) + Y - 40.f;
	const float CenterX = X; 
	mat EffectWorld = MATH::WorldMatrix({ XSize,YSize,0 }, { 0,0,90.0f },{ CenterX+50.f, 300.f+FloorY,0.0f });

	_Effect.SetVSConstantData(m_pDevice, "World", EffectWorld);

	m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), std::get<0>(TextureTuple));

	_Effect.SetPSConstantData(m_pDevice, "bUI", 1l);
	_Effect.SetPSConstantData(m_pDevice, "Shine", 20.f);

	_VertexBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, ZWriteEnableFlag);
}

void CPlayer::PushLightFromName(const std::wstring& LightName , const float Duration)&
{
	bool bIsValidName = false;

	if (LightName == L"ShotGunShot")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =	MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,1,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"AkimboFire")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,1,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 100.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"MagnumFire")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,1,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 200.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"StaffFire")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 0,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"StaffCharge")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 0,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"StaffRelease")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =	MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 0,0,0,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}

	if (LightName == L"ElectricStaffFire")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);

		size_t CurrentDamageStep = 0l;
		if (CurrentElectricStaffFireDamage <= ElectricStaffDamageLimitTable[0])
		{
			CurrentDamageStep = 0l;
			_Light.Diffuse = { 0.4f,0.733f,0.733f,1 };
		}
		else if (CurrentElectricStaffFireDamage <= ElectricStaffDamageLimitTable[1])
		{
			CurrentDamageStep = 1ul;
			_Light.Diffuse = { 119.f/255.f,102.f/255.f,204.f/255.f,1 };
		}
		else 
		{
			CurrentDamageStep = 2ul;
			_Light.Diffuse = { 136.f/255.f,51.f/255.f,51.f/255.f,1 };
		};

		/*const float Lhs = CurrentElectricStaffFireDamage - ElectricStaffDamageLimitTable[CurrentDamageStep - 1ul] ;
		float Rhs = ElectricStaffDamageLimitTable[CurrentDamageStep] - ElectricStaffDamageLimitTable[CurrentDamageStep-1ul];
		if (MATH::almost_equal(Rhs, 0.0f)) Rhs += (std::numeric_limits<float>::min)();
		_Light.Diffuse *= Lhs / Rhs;*/

		_Light.Diffuse *= CurrentElectricStaffFireDamage* ElectricStaffLightDiffuseCoefft;
		_Light.Diffuse.w = 1.f;

		_Light.Diffuse.w = 1.f;
		_Light.Priority = 1l;
		_Light.Radius = CurrentElectricStaffFireDamage * ElectricStaffLightRadiusCoefft;
		Effect::RegistLight(std::move(_Light));
}
	if (LightName == L"FlakFire")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 255.f/255.f,51.f / 255.f,51.f / 255.f,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"SpellLight")
	{
		const float Coefft = 1.f - (Duration / SpellLightDuration);
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location = MATH::ConvertVec4(m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.0f, 1.f);
		const vec4 Diffuse = MATH::ConvertVec4(vec3{ 2.f,2.f,2.f } *Coefft, 1.f);
		_Light.Diffuse = Diffuse;
		_Light.Priority = 1l;
		_Light.Radius = SpellLightRadius * Coefft;
		Effect::RegistLight(std::move(_Light));
	}
#ifdef _DEBUG
	if (!bIsValidName == true)
	{
		PRINT_LOG(L"Warning!", L"Not Valid Player Lighting Name!");
	}
#endif 
}

void CPlayer::ShieldStart()&
{
	RemainShield = 100.0f;
}
uint8_t CPlayer::CalcShieldStep() const&
{
	if (RemainShield >= 60.0f) 
	{
		return 0;
	}
	else if (RemainShield >= 50.0f)
	{
		return 1;
	}
	else if (RemainShield >= 35.0f)
	{
		return 2;
	}
	else if (RemainShield >= 20.0f)
	{
		return 3;
	}
	else if (RemainShield >= 5.0f)
	{
		return 4;
	}
	else if (RemainShield >= 3.0f)
	{
		return 5;
	}
	else if (RemainShield >= 1.0f)
	{
		return 6;
	}
	else if (RemainShield > 0.0f)
	{
		return 7;
	}
	else
	{
		return 8u;
	}

	return 8u;
}
void CPlayer::PlayStepSound()
{
	++m_iStepIndex;
	if (m_iStepIndex > 3)
		m_iStepIndex = 0;
	TCHAR szSoundKey_Step[64] = L"";
	swprintf_s(szSoundKey_Step, L"Step%d.wav", m_iStepIndex);

	CSoundMgr::Get_Instance()->PlaySound(szSoundKey_Step, CSoundMgr::PLAYER_STEP);
}
const vec3 CPlayer::CalcElectricStaffGizmo() const&
{
	return m_pTransformCom->GetLocation()+ (m_pTransformCom->GetUp() * -0.18f) + (m_pTransformCom->GetRight() * 0.122f);
}



