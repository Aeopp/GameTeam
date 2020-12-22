#include "stdafx.h"
#include "..\Headers\Player.h"
#include "ImGuiHelper.h"
#include "CollisionComponent.h"
#include "Vertexs.h"
#include "Monster.h"
#include "DXWrapper.h"
#include "NormalUVVertexBuffer.h"
#include "ParticleSystem.h"

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



	return S_OK;
}

HRESULT CPlayer::ReadyGameObject(void* pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

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
	for (auto& _LightDurationPair : LightingDurationTable)
	{
		float& _Duration = _LightDurationPair.second;
		_Duration -= fDeltaTime;
		if (_Duration > 0)
		{
			PushLightFromName(_LightDurationPair.first);
		}
	};

	MyLight _Light;
	_Light.Diffuse = { 1,1,1,1 };
	_Light.Location = MATH::ConvertVec4(m_pTransformCom->GetLocation(), 1.f);
	_Light.Radius = 60.f;
	_Light.Priority = 0l;
	Effect::RegistLight(std::move(_Light));

	if (_AnimationTextures.GetAnimationKey() == L"Staff_Loop")
	{
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 200.f;
		Effect::RegistLight(std::move(_Light));
	}

	return _uint();
}

_uint CPlayer::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::ParticleAfterAlpha
		, this)))
		return 0;

	return _uint();
}

HRESULT CPlayer::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	/*m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); 
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");
	vec3 GunLocation = m_pTransformCom->GetLocation() +  (m_pTransformCom->GetLook() *1.7f);
	GunLocation.y -= 0.25f;
	vec3 GunScale = m_pTransformCom->GetScale();
	vec3 GunRotation = m_pTransformCom->GetRotation();

	//GunRotation.y +=180.f;

	mat GunWorld = MATH::WorldMatrix(GunScale, GunRotation, GunLocation);
	_Effect.SetVSConstantData(m_pDevice, "World", GunWorld);

	const auto& TextureTuple = _AnimationTextures.GetCurrentTexture();

	m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),std::get<0> (TextureTuple));
	m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"), std::get<1>(TextureTuple));
	m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"), std::get<2>(TextureTuple));
	
	_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0 );
	_Effect.SetPSConstantData(m_pDevice,"bNormalSamplerBind", 0);
	_Effect.SetPSConstantData(m_pDevice, "Shine", 20.f);
	
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();
	
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	_CollisionComp->DebugDraw();

	return S_OK;
}

void CPlayer::Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo)
{
	HP -= 1.f;
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
	case CPlayer::EWeaponState::ShotGun:
		ShotGunReload();
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

HRESULT CPlayer::AddStaticComponents()
{
	Super::AddStaticComponents();

	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Radius = 2.f;
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

static auto PlaneEffect = [](CPlayer& _Player, const vec3 IntersectPoint, vec3 Normal ,float Scale)
{
	Particle _Particle;
	_Particle.StartLocation = IntersectPoint + Normal * 0.1f;
	_Particle.Location = IntersectPoint + Normal * 0.1f;
	_Particle.Delta = FLT_MAX;
	_Particle.Durtaion = 1000.f;
	_Particle.EndFrame = 1ul;
	_Particle.Name = L"BulletHole" + std::to_wstring(MATH::RandInt({ 0,3 }));
	_Particle.Scale = { Scale,Scale,Scale };

	Normal = MATH::Normalize(Normal);
	const vec3 Axis = MATH::Cross({ 0,0,-1 }, Normal);
	float Degree = MATH::ToDegree(std::acosf(MATH::Dot(Normal, vec3{ 0,0,-1 })));
	//if (MATH::almost_equal(Axis.y, 1.f) || MATH::almost_equal(Axis.y, 0.0f))
	//{
	//	Degree = 0.f;
	//};

	mat Rot = MATH::RotationMatrixFromAxis(Axis, Degree);
	_Particle.bRotationMatrix = true;
	_Particle.RotationMatrix = Rot;
	ParticleSystem::Instance().PushParticle(std::move(_Particle));
};

void CPlayer::ShotGunShot()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[2ul] = [this]()
	{
		ShotGunReload();

		/*AnimationTextures::NotifyType _Notify;
		_Notify[14ul] = [this]()
		{
			_AnimationTextures.ChangeAnim(L"ShotGun_Idle", FLT_MAX, 1ul);
		};
		_AnimationTextures.ChangeAnim(L"ShotGun_Reload", 0.07f, 14ul, false, std::move(_Notify));*/
	};

	_AnimationTextures.ChangeAnim(L"ShotGun_Shot", 0.1f, 2ul,
		false, std::move(_Notify));

	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);
//	_Ray.Start = m_pTransformCom->GetLocation();

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
				Collision::Info _CollisionInfo = IsCollision.second;
				_CurrentMonster->Hit(this, std::move(_CollisionInfo));
			}
		}
	}

	LightingDurationTable[L"ShotGunShot"] = 0.3f;


	

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
}

void CPlayer::ShotGunReload()
{
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
		_CollisionParticle.Durtaion = 1000.f;
		_CollisionParticle.Name = L"ShotGunShell";
		_CollisionParticle.Radius = 0.2f;
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


	for (size_t i = 0; i < 100; ++i)
	{
		POINT _Pt;
		GetCursorPos(&_Pt);
		ScreenToClient(g_hWnd, &_Pt);
		vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
		Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);
	//	_Ray.Start = m_pTransformCom->GetLocation() + (_Ray.Direction * i);

		Particle _Partice;
		_Partice.Delta = 2;
		_Partice.Durtaion = 2 * 1;
		_Partice.EndFrame = 1;
		_Partice.Location = m_pTransformCom->GetLocation() + (_Ray.Direction * i);
		_Partice.AlphaLerp = (i/1.f) / 100.f;
		//_Partice.Rotation.y = 75.f;
		//_Partice.Rotation.x = 80.f;

		_Partice.Scale = { 0.5f, 0.5f,0.5f };
		_Partice.Name = L"DaggerThrow";

		_Partice.Dir = _Ray.Direction;

		_Partice.Speed = 10.f;

		ParticleSystem::Instance().PushParticle(_Partice);
	}

}

void CPlayer::AkimboFire()
{
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Akimbo_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Akimbo_Fire", 0.025f, 4ul, false, std::move(_Notify));

	LightingDurationTable[L"AkimboFire"] = 0.2f;

	
	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);


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
				Collision::Info _CollisionInfo = IsCollision.second;
				_CurrentMonster->Hit(this, std::move(_CollisionInfo));
			}
		}
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
	_CollisionParticle.Rotation = { 0.f,0.f,MATH::RandReal({-360,360}) };
	_CollisionParticle.Durtaion = 1000.f;
	_CollisionParticle.Name = L"BulletShell";
	_CollisionParticle.Radius = 0.1f;
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
	AnimationTextures::NotifyType _Notify;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Magnum_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Magnum_Fire", 0.07f, 4ul, false, std::move(_Notify));

	LightingDurationTable[L"MagnumFire"] = 0.2f;

	POINT _Pt;
	GetCursorPos(&_Pt);
	ScreenToClient(g_hWnd, &_Pt);
	vec3 ScreenPos{ (float)_Pt.x,(float)_Pt.y,1.f };
	Ray _Ray = MATH::GetRayScreenProjection(ScreenPos, m_pDevice, WINCX, WINCY);

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
				Collision::Info _CollisionInfo = IsCollision.second;
				_CurrentMonster->Hit(this, std::move(_CollisionInfo));
			}
		}
	};

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
	_CollisionParticle.Durtaion = 1000.f;
	_CollisionParticle.Name = L"MagnumShell";
	_CollisionParticle.Radius = 0.2f;
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
	AnimationTextures::NotifyType _Notify;
	bStaffLoop = false;

	_Notify[4ul] = [this]()
	{
		_AnimationTextures.ChangeAnim(L"Staff_Idle", FLT_MAX, 1);
	};

	_AnimationTextures.ChangeAnim(L"Staff_Fire", 0.07f, 4ul, false, std::move(_Notify));

	LightingDurationTable[L"StaffFire"] = 0.15f;
}

void CPlayer::StaffCharge()
{
	AnimationTextures::NotifyType _Notify;
	bStaffLoop = false;

	_Notify[16ul] = [this]()
	{
		StaffLoop();
	};

	_AnimationTextures.ChangeAnim(L"Staff_Charge", 0.07f, 16ul, false, std::move(_Notify));

	LightingDurationTable[L"StaffCharge"] = 1.1f;
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
}

void CPlayer::StaffLoop()
{
	_AnimationTextures.ChangeAnim(L"Staff_Loop", 0.07f, 10, true);
	// 공격 키 눌렀을때 웨폰 상태가 스태프라면 Fire 하고 난 다음에 
	// bStaffLoop =false 로 만들기
	// Key Up 일때 Loop 였다면 bStaffLoop = false 하고 Release 호출
	bStaffLoop = true;

}

void CPlayer::PushLightFromName(const std::wstring& LightName)&
{
	bool bIsValidName = false;

	if (LightName == L"ShotGunShot")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
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
		_Light.Diffuse = { 1,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 200.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"StaffCharge")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	if (LightName == L"StaffRelease")
	{
		bIsValidName = true;
		MyLight _Light{};
		_Light.Location =
			MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
		_Light.Diffuse = { 1,0,1,1 };
		_Light.Priority = 1l;
		_Light.Radius = 400.f;
		Effect::RegistLight(std::move(_Light));
	}
	//if (LightName == L"StaffLoop")
	//{
	///*	bIsValidName = true;
	//	MyLight _Light{};
	//	_Light.Location =
	//		MATH::ConvertVec4((m_pTransformCom->GetLocation() + m_pTransformCom->GetLook() * 10.f), 1.f);
	//	_Light.Diffuse = { 0.556862f,0.317f,1,1 };
	//	_Light.Priority = 1l;
	//	_Light.Radius = 150.f;
	//	Effect::RegistLight(std::move(_Light));*/
	//}

#ifdef _DEBUG
	if (!bIsValidName == true)
	{
		PRINT_LOG(L"Warning!", L"Not Valid Player Lighting Name!");
	}
#endif 
}
