#include "stdafx.h"
#include "..\Headers\Player.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "Vertexs.h"
#include "Monster.h"



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

HRESULT CPlayer::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	_Quad=SubSetInfo::GetMeshFromObjFile
	(m_pDevice, L"..\\Resources\\Mesh\\Quad");

	// Harvester 로딩
	{
		_AnimationTextures._TextureMap[L"Harvester_Fire"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Harvester\\Fire\\", 3);

		_AnimationTextures._TextureMap[L"Harvester_Idle"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Harvester\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Harvester_Hud"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Harvester\\Hud\\", 1);

		_AnimationTextures._TextureMap[L"Harvester_Reload"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Harvester\\Reload\\", 23);
	}

	{
		_AnimationTextures._TextureMap[L"Dagger_Stab"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Stab\\", 4);

		_AnimationTextures._TextureMap[L"Dagger_Idle"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Dagger_Hud"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Hud\\", 1);

		_AnimationTextures._TextureMap[L"Dagger_Throw"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Dagger\\Throw\\", 12);

		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX, 1);
	}

	{
		_AnimationTextures._TextureMap[L"Magnum_Fire"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Fire\\", 4);

		_AnimationTextures._TextureMap[L"Magnum_Idle"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Magnum_Hud"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Magnum\\Hud\\", 1);
	}

	{
		_AnimationTextures._TextureMap[L"Akimbo_Fire"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Fire\\", 4);

		_AnimationTextures._TextureMap[L"Akimbo_Idle"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Akimbo_Hud"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Akimbo\\Hud\\", 1);
	}

	{
		_AnimationTextures._TextureMap[L"Staff_Fire"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Fire\\", 4);

		_AnimationTextures._TextureMap[L"Staff_Hud"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Hud\\", 1);

		_AnimationTextures._TextureMap[L"Staff_Charge"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Charge\\", 16);

		_AnimationTextures._TextureMap[L"Staff_Loop"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Loop\\", 10);

		_AnimationTextures._TextureMap[L"Staff_Idle"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Idle\\", 1);

		_AnimationTextures._TextureMap[L"Staff_Release"] = CreateTextures(
			m_pDevice, L"..\\Resources\\Player\\Staff\\Release\\", 5);
	}



	return S_OK;
}

HRESULT CPlayer::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.fSpeedPerSec = 10;
	m_pTransformCom->m_TransformDesc.fRotatePerSec = MATH::PI;
	m_pTransformCom->m_TransformDesc.vPosition = { 0,10,0 };
	m_pTransformCom->m_TransformDesc.vRotation = { 0,0,0 };
	m_pTransformCom->m_TransformDesc.vScale = { 1,1,1 };

	return S_OK;
}

_uint CPlayer::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

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

	return _uint();
}

_uint CPlayer::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	return _uint();
}

HRESULT CPlayer::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");
	vec3 GunLocation = m_pTransformCom->GetLocation() +  (m_pTransformCom->GetLook() *2.f);
	GunLocation.y -= 0.36f;
	vec3 GunScale = m_pTransformCom->GetScale();
	GunScale.y *= -1.f;

	const vec3 GunRotation = m_pTransformCom->GetRotation();
	mat GunWorld = MATH::WorldMatrix(GunScale, GunRotation, GunLocation);
	_Effect.SetVSConstantData(m_pDevice, "World", GunWorld);

	for (auto& _CurrentSubSet : *_Quad)
	{
		m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),
			_AnimationTextures.GetCurrentTexture());
		_Effect.SetPSConstantData(m_pDevice, "Shine", 20.f);
		m_pDevice->SetStreamSource(0, _CurrentSubSet.VtxBuf, 0,sizeof(Vertex::Texture));
		m_pDevice->SetVertexDeclaration(_CurrentSubSet.Decl);
		m_pDevice->SetVertexShader(_Effect.VsShader);
		m_pDevice->SetPixelShader(_Effect.PsShader);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _CurrentSubSet.TriangleCount);
	}
	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	_CollisionComp->DebugDraw();

	return S_OK;
}

void CPlayer::MoveForward(const float DeltaTime)&
{
	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Forward{ world._31,0.f,world._33 };
	Forward = MATH::Normalize(Forward);
	const float Speed = Desc.fSpeedPerSec;
	Desc.vPosition += Forward * Speed * DeltaTime;
};

void CPlayer::MoveRight(const float DeltaTime)&
{
	auto& Desc = m_pTransformCom->m_TransformDesc;
	const mat world = Desc.matWorld;
	vec3 Right{ world._11,0.f,world._13 };
	Right = MATH::Normalize(Right);
	const float Speed = Desc.fSpeedPerSec;
	Desc.vPosition += Right * Speed * DeltaTime;
}
void CPlayer::MouseRight()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		DaggerThrow();
		break;
	case CPlayer::EWeaponState::Harvester:
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
	case CPlayer::EWeaponState::Harvester:
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
	case CPlayer::EWeaponState::Harvester:
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
	case CPlayer::EWeaponState::Harvester:
		HarvesterFire();
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
	case CPlayer::EWeaponState::Harvester:
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
	default:
		break;
	};
};

void CPlayer::RButtonEvent()&
{
	switch (_CurrentWeaponState)
	{
	case CPlayer::EWeaponState::Dagger:
		break;
	case CPlayer::EWeaponState::Harvester:
		HarvesterReload();
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
	_CurrentWeaponState = EWeaponState::Harvester;
	_AnimationTextures.ChangeAnim(L"Harvester_Idle", FLT_MAX, 1);
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

HRESULT CPlayer::AddStaticComponents()
{
	Super::AddStaticComponents();

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::Player;
	_Info.bMapCollision = true;
	_Info.Owner = this;

	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

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
	CGameObject::Free();
}

void CPlayer::HarvesterFire()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[3ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Harvester_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Harvester_Fire", 0.1f, 3ul,
		false, std::move(_Notify));

	Ray _Ray;
	_Ray.Start = m_pTransformCom->GetLocation();
	_Ray.Direction = MATH::Normalize(m_pTransformCom->GetLook());

	auto _MonsterList =m_pManagement->GetGameObjects(-1, L"Layer_Monster");

	for (auto& _CurrentMonster : _MonsterList)
	{
		auto _Component = _CurrentMonster->GetComponent
		(CComponent::Tag + TYPE_NAME<CCollisionComponent >());
		
			auto _CollisionComp = dynamic_cast<CCollisionComponent*> (_Component);
		if (_CollisionComp)
		{
			float t0 = 0;
			float t1 = 0;
			std::pair<bool, Engine::Collision::Info>
				IsCollision = Collision::IsRayToSphere(_Ray,
					_CollisionComp->_Sphere, t0, t1);

			if (IsCollision.first)
			{
				Collision::Info _CollisionInfo = IsCollision.second;
				_CurrentMonster->Hit(this, std::move(_CollisionInfo));
			}
		}
		
	}
}

void CPlayer::HarvesterReload()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[23ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Harvester_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Harvester_Reload", 0.07f, 23ul,
		false, std::move(_Notify));
}

void CPlayer::DaggerStab()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX,1);
	};

	_AnimationTextures.ChangeAnim(L"Dagger_Stab", 0.07f, 4ul, false,
		std::move(_Notify));
}

void CPlayer::DaggerThrow()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[12ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Dagger_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Dagger_Throw", 0.07f, 12ul, false,
		std::move(_Notify));
}

void CPlayer::AkimboFire()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Akimbo_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Akimbo_Fire", 0.04f, 4ul, false, std::move(_Notify));
}

void CPlayer::MagnumFire()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Magnum_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Magnum_Fire", 0.07f, 4ul, false, std::move(_Notify));
}

void CPlayer::StaffFire()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Staff_Fire", 0.07f, 4ul, false, std::move(_Notify));
}

void CPlayer::StaffCharge()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[16ul] = [this]()
	{
		StaffLoop();
	};

	_AnimationTextures.ChangeAnim(L"Staff_Charge", 0.07f, 16ul, false, std::move(_Notify));
}

void CPlayer::StaffRelease()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[5ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1, true);
	};

	_AnimationTextures.ChangeAnim(L"Staff_Release", 0.07f, 5ul,
		false, std::move(_Notify));
}

void CPlayer::StaffLoop()
{
	_AnimationTextures.ChangeAnim(L"Staff_Loop", 0.07f, 10, true);
	// 공격 키 눌렀을때 웨폰 상태가 스태프라면 Fire 하고 난 다음에 
	// bStaffLoop =false 로 만들기
	// Key Up 일때 Loop 였다면 bStaffLoop = false 하고 Release 호출
	bStaffLoop = true;
}
