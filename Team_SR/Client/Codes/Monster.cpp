#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Camera.h"
#include "NormalUVVertexBuffer.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "UIManager.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrameCnt(0.f), m_fStartFrame(0.f), m_fEndFrame(0.f), m_fFrameSpeed(10.f)
	, m_fCrossValue(0.f), m_vCollisionDir{0.f, 0.f, 0.f}, m_vAim {0.f, 0.f, 0.f}
	, m_pPlayer(nullptr), m_stOriginStatus{}, m_stStatus{}, m_wstrTextureKey(L""), m_pJumpPointSearch(JumpPointSearch::Get_Instance())
	, m_bFrameLoopCheck(false), m_bNoLoop(false), m_byMonsterFlag(0)
{
	bGravity = true;
}

HRESULT CMonster::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(CMonster::AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		// 구조체 크기 검사
		if (sizeof(MonsterBasicArgument) == *static_cast<_uint*>(pArg))
		{
			MonsterBasicArgument* pArgument = static_cast<MonsterBasicArgument*>(pArg);
			m_pPlayer = reinterpret_cast<CGameObject*>(pArgument->pPlayer);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			// 동적 생성된 거임
			if (pArgument->bDeleteFlag) {
				delete pArg;
				// 플레이어 추적 ON
				// 동적 생성된 몬스터는 바로 플레이어를 추적한다
				m_byMonsterFlag |= static_cast<BYTE>(MonsterFlag::PlayerTracking);
			}
		}
	}

	//for (size_t i = 0; i < 52u; ++i)
	//{
	//	GibTable.push_back(i);
	//};

	return S_OK;
}

_uint CMonster::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);
	_CurrentDeltaTime = fDeltaTime;
	// 2020.12.17 11:08 KMJ
	// 충돌 이동
	//CollisionMovement(fDeltaTime);

	return _uint();
}

_uint CMonster::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	IsBillboarding();

	FloorBloodCurrentCoolTime -= fDeltaTime;
	LightHitTime -= fDeltaTime;
	FreezeDeadProcessTime -= fDeltaTime;
	FreezeBloodParticleTime -= fDeltaTime;

	if(IsDead())
	{
		bGravity = false;
		_CollisionComp->bCollision = false;
		_CollisionComp->bWallCollision = false;
		_CollisionComp->bFloorCollision = false;
	};

	return _uint();
}

HRESULT CMonster::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)m_pManagement->GetGameObject((_int)-1, L"Layer_MainCamera");
	if (nullptr == pCamera)
		return E_FAIL;

	const auto& _TransformDesc =m_pTransformCom->m_TransformDesc;
	vec3 BillboardRotation = _TransformDesc.vRotation;
	BillboardRotation.y += pCamera->GetTransform()->GetRotation().y;
	m_pTransformCom->m_TransformDesc.matWorld = MATH::WorldMatrix(_TransformDesc.vScale, BillboardRotation, _TransformDesc.vPosition);

	/*_matrix matBillboardY, matView;
	D3DXMatrixIdentity(&matBillboardY);
	matView = pCamera->GetCameraDesc().matView;

	matBillboardY._11 = matView._11;
	matBillboardY._13 = matView._13;
	matBillboardY._31 = matView._31;
	matBillboardY._33 = matView._33;

	D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

	m_pTransformCom->m_TransformDesc.matWorld *= matBillboardY;*/

	return S_OK;
}


HRESULT CMonster::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	const mat World = m_pTransformCom->m_TransformDesc.matWorld;
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");

	// 현재 사용중이던 텍스쳐를 여기에 세팅.
	{
		//  본래 사용중이던 로직 그대로 현재 텍스쳐를 구해와서 세팅 .
		{
			auto iter_find = m_mapTexture.find(m_wstrTextureKey);
			if (m_mapTexture.end() == iter_find)
				return E_FAIL;
			CTexture* pTexture = (CTexture*)iter_find->second;
			IDirect3DBaseTexture9* const  DiffuseTexture = pTexture->GetTexture((_uint)m_fFrameCnt);

			m_pDevice->SetTexture(_Effect.GetTexIdx("DiffuseSampler"),DiffuseTexture);
		}
		// 1.       그냥 세팅을 안하거나
		{
			_Effect.SetPSConstantData(m_pDevice, "bSpecularSamplerBind", 0l);
			_Effect.SetPSConstantData(m_pDevice, "bNormalSamplerBind", 0l);
		}
		// 2. 세팅을 하고 난 이후의                                   ↑↑↑↑↑↑↑↑↑↑     TRUE 로 바꾸어주기.
		{
			// m_pDevice->SetTexture(_Effect.GetTexIdx("SpecularSampler"),SpecularTexture);
			// m_pDevice->SetTexture(_Effect.GetTexIdx("NormalSampler"),NormalTexture);
		}
	}
	// 월드 행렬 바인딩
	_Effect.SetVSConstantData(m_pDevice, "World", World);
	// 광택 설정 
	_Effect.SetPSConstantData(m_pDevice, "Shine", Shine);
	m_pDevice->SetVertexShader(_Effect.VsShader);
	m_pDevice->SetPixelShader(_Effect.PsShader);
	_VertexBuffer->Render();

	if (_CollisionComp)
	{
		_CollisionComp->DebugDraw();
	}

	return S_OK;
}

HRESULT CMonster::AddComponents()
{

	if (FAILED(CGameObject::AddComponent(
		(_uint)ESceneID::Static,
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		CComponent::Tag + TYPE_NAME<CNormalUVVertexBuffer>(),
		(CComponent**)&_VertexBuffer)))
		return E_FAIL;

	return S_OK;
}

// 몬스터가 피해를 받음
void CMonster::Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo)
{
	CGameObject::Hit(_Target, _CollisionInfo);
	
	// 2020.12.17 10:44 KMJ
	// 공평회에서는 일단 고정임d
	//m_stStatus.fHP -= fDemage;
	 m_stStatus.fHP-=_Target->CurrentAttack;
	 auto _Player = dynamic_cast<const CPlayer* const>(_Target);
	 const CPlayer::EWeaponState _WeaponState=_Player->GetWeaponState();

	 if (false == (_WeaponState == CPlayer::EWeaponState::ElectricStaff ||
					_WeaponState == CPlayer::EWeaponState::Flak))
	 {
		 BloodParticle();
	 }

	 //_int iMax = (_int)m_stOriginStatus.fHP;
	 //_int iMin = (_int)m_stStatus.fHP;

	 //CUIManager::Get_Instance()->OnMonsterBar(&iMax, &iMin);
	 CUIManager::Get_Instance()->OnMonsterBar(&m_stOriginStatus.fHP, &m_stStatus.fHP);

	 if (m_stStatus.fHP < 0.f)
	 {
		 DeadProcess();
	 }
}

void CMonster::MapHit(const PlaneInfo& _PlaneInfo, const Collision::Info& _CollisionInfo)
{
	CGameObject::MapHit(_PlaneInfo, _CollisionInfo);

	if (_CollisionInfo.Flag == L"Floor")
	{
		bGravity = false;
	};
}

void CMonster::ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)
{
	CGameObject::ParticleHit(_Particle, _CollisionInfo);

	if (_Particle)
	{
		CollisionParticle* _ParticlePtr = reinterpret_cast<CollisionParticle*>(_Particle);

		m_stStatus.fHP -= _ParticlePtr->CurrentAttack;

		//_int iMax = (_int)m_stOriginStatus.fHP;
		//_int iMin = (_int)m_stStatus.fHP;

		//CUIManager::Get_Instance()->OnMonsterBar(&iMax, &iMin);
		CUIManager::Get_Instance()->OnMonsterBar(&m_stOriginStatus.fHP, &m_stStatus.fHP);

		if (m_stStatus.fHP < 0.f)
		{
			DeadProcess();
		}

		BloodParticle();
	}
};

void CMonster::FlashHit()&
{
	LightHitTime = 0.1f;
}

void CMonster::FreezeHit()
{
	m_stStatus.fHP -= (FreezeHitDamage * _CurrentDeltaTime);

	//_int iMax = (_int)m_stOriginStatus.fHP;
	//_int iMin = (_int)m_stStatus.fHP;

	//CUIManager::Get_Instance()->OnMonsterBar(&iMax, &iMin);
	CUIManager::Get_Instance()->OnMonsterBar(&m_stOriginStatus.fHP, &m_stStatus.fHP);

	if (m_stStatus.fHP < 0.0f && FreezeDeadProcessTime<0.0f)
	{
		FreezeDeadProcessTime = 0.7f;
		DeadProcess();
	}
	if (FreezeBloodParticleTime < 0.0f)
	{
		FreezeBloodParticleTime = 0.4f;
		BloodParticle();
	}
};

bool CMonster::Attack(const Sphere _Sphere, const float Attack)&
{
	auto _Player = dynamic_cast<CPlayer* const>(m_pManagement->GetGameObject(-1, L"Layer_Player", 0));
	if (false == _Player->_CollisionComp->bCollision)return false;

	Sphere TargetSphere = _Player->_CollisionComp->_Sphere;
	auto OCollision = Collision::IsSphereToSphere(_CollisionComp->_Sphere, TargetSphere);
	if (OCollision.first)
	{
		this->CurrentAttack = Attack;
		_Player->Hit(this, OCollision.second);
		return true;
	};
	return false;
};

bool CMonster::Attack(const Ray _Ray, const float Attack)&
{
	auto _Player = dynamic_cast<CPlayer* const>(m_pManagement->GetGameObject(-1, L"Layer_Player", 0));
	if (false == _Player->_CollisionComp->bCollision)return false;

	Sphere TargetSphere = _Player->_CollisionComp->_Sphere;
	float t0, t1;
	vec3 IntersectPoint;
	auto OCollision = Collision::IsRayToSphere(_Ray, TargetSphere, t0, t1, IntersectPoint);
	if (OCollision.first)
	{
		this->CurrentAttack = Attack;
		_Player->Hit(this, OCollision.second);
		return true;
	};
}
void CMonster::MeleeAttack()
{
	_vector AttackDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	D3DXVec3Normalize(&AttackDir, &AttackDir);
	Ray _Ray;
	_Ray.Direction = AttackDir;
	_Ray.Start = m_pTransformCom->m_TransformDesc.vPosition;
	CMonster::Attack(_Ray, m_stStatus.fMeleeRange * 0.5);
}

// 텍스처 프레임 이동 - 프레임 카운트가 End에 도달하면 true, 아니면 false
bool CMonster::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		// 2021.01.11 KMJ
		// 깜빡임 현상 수정
		if (m_bNoLoop) {
			m_fFrameCnt = m_fEndFrame - 1.f;
			return true;
		}
		m_fFrameCnt = m_fStartFrame;
		return true;;
	}
	return false;
}

// 플레이어 인식 - 인식하면 true, 인식하지 못하면 false
bool CMonster::PlayerAwareness()
{
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	float fDis = D3DXVec3Length(&vDir);
	// 플레이어가 범위 안에 있으면
	if (fDis <= m_stStatus.fDetectionRange) {
		vec3 DirFromXZPlane = vDir;
		DirFromXZPlane.y = 0.0f;
		DirFromXZPlane = MATH::Normalize(DirFromXZPlane);
		RotationXZPlane = MATH::ToDegree(std::acosf(MATH::Dot(DirFromXZPlane, vec3{ 0.0f,0.0f,1.0f })));
		return true;
	}
	return false;
}

// 플레이어가 가까이 근접해있는가 - 가까우면 true, 아니면 false
bool CMonster::PlayerBeNear()
{
	vec3 vDir = m_pPlayer->GetTransform()->m_TransformDesc.vPosition - m_pTransformCom->m_TransformDesc.vPosition;
	float fDis = D3DXVec3Length(&vDir);
	// 플레이어가 범위 안에 있으면
	if (fDis <= m_stStatus.fMeleeRange) 
	{
		vec3 DirFromXZPlane = vDir;
		DirFromXZPlane.y = 0.0f;
		DirFromXZPlane = MATH::Normalize(DirFromXZPlane);
		RotationXZPlane = MATH::ToDegree(std::acosf(MATH::Dot(DirFromXZPlane, vec3{ 0.0f,0.0f,1.0f })));
		return true;
	}
	return false;
}

// 2020.12.17 11:08 KMJ
// 충돌 이동
void CMonster::CollisionMovement(float fDeltaTime)
{
	// 개쌉 야매
	m_fCrossValue -= 10.f * fDeltaTime;
	if (m_fCrossValue < 0.f)
		m_fCrossValue = 0.f;
	m_pTransformCom->m_TransformDesc.vPosition += m_vCollisionDir * m_fCrossValue * fDeltaTime;
}

void CMonster::CreateBlood()
{
	//m_pManagement->AddScheduledGameObjectInLayer(
	//	(_int)ESceneID::Static,
	//	CGameObject::Tag + L"Blood",
	//	L"Layer_Blood",
	//	nullptr, (void*)&m_pTransformCom->m_TransformDesc.vPosition);
}

void CMonster::CreateFloorBlood()
{
}

// 길찾기
// _vDepa : 출발지
// _vDest : 도착지
void CMonster::PathFinding(vec3 _vDepa, vec3 _vDest)
{
	int iCount = 3;

	// 길찾기 시작
	while (!m_pJumpPointSearch->Start(_vDepa, _vDest)) {
		// 탐색 실패시 목적지를 근처의 경로로 변경 후
		// 다시 길찾기를 한다
		JumpPointSearch::Get_Instance()->NearbyPath(_vDepa, _vDest);
		--iCount;
		if (iCount == 0) {
			break;
		}
	}
	// 길찾기 완료
	// 이동 지점 리스트에 담기
	m_listMovePos.clear();
	m_pJumpPointSearch->Finish(m_listMovePos);
}

static void FloorBlood(const PlaneInfo& _PlaneInfo,const vec3 IntersectPoint)
{
	const vec3 Normal = { 0,0,-1 };

	vec3 PlaneNormal = 
	{
	_PlaneInfo._Plane.a,
	_PlaneInfo._Plane.b,
	_PlaneInfo._Plane.c
	};

	PlaneNormal = MATH::Normalize(PlaneNormal);

	const vec3 Axis = (MATH::Cross(Normal, PlaneNormal));
	const float Radian = std::acosf(MATH::Dot(Normal, PlaneNormal));
	mat RotAxis;
	D3DXMatrixRotationAxis(&RotAxis, &Axis, Radian);

	Particle _Particle;
	_Particle.bBillboard = false;
	_Particle.bLoop = false;
	_Particle.bMove = false;
	_Particle.bRotationMatrix = true;
	_Particle.RotationMatrix = RotAxis;
	_Particle.UVAlphaLerp = false;
	int32_t Frame = MATH::RandInt({ 0,3 }); 
	_Particle.CurrentFrame = Frame; 
	_Particle.CurrentT = static_cast<float>(Frame); 
	_Particle.Delta = FLT_MAX;
	_Particle.Durtaion = 180.0f;
	_Particle.EndFrame = Frame;
	_Particle.StartLocation =   _Particle.Location = IntersectPoint + PlaneNormal * 0.01f;
	_Particle.Scale = { 1.3,1.3,1.3 };
	_Particle.Name = L"FloorBlood";

	ParticleSystem::Instance().PushParticle(std::move(_Particle)  );
};

void CMonster::BloodParticle()
{
	Particle _Particle;
	_Particle.bBillboard = true;
	_Particle.bLoop = false;
	_Particle.bMove = false;
	_Particle.Delta = 0.08f;

	uint32_t EndFrame{ 0ul };  
	std::wstring Name{}; 

	if (m_stStatus.fHP <= 0.f)
	{
		EndFrame = 18ul;
		Name = L"OverKill";
	}
	else
	{
		int TextNumber = MATH::RandInt({ 1,2 });
		Name = L"BloodBigHit" + std::to_wstring(TextNumber);
		EndFrame = 8ul;
	};

	_Particle.Name = std::move(Name);
	_Particle.EndFrame = EndFrame;
	_Particle.Durtaion = static_cast<float> (EndFrame) * _Particle.Delta;
	_Particle.Scale = { 2.25f,2.25f,2.25f };
	_Particle.Location = m_pTransformCom->GetLocation() + -m_pTransformCom->GetLook() * 1.f;
	ParticleSystem::Instance().PushParticle(_Particle);

	for (size_t i = 0; i < 26; ++i)
	{
		Particle _Particle;
		_Particle.bBillboard = true;
		_Particle.bLoop = true;
		_Particle.bMove = true;
		_Particle.Delta = FLT_MAX;
		_Particle.Gravity = 5.f;

		const float Speed = MATH::RandReal({ 5,10});
		_Particle.Dir = MATH::RandVec();
		_Particle.Durtaion =  4.f;
		_Particle.Angle = MATH::RandReal({ 90,130});
		_Particle.Durtaion = 2.f;
		_Particle.EndFrame = 1ul;
		_Particle.Scale = { 0.15f,0.15f,0.15f };
		_Particle.StartLocation = _Particle.Location = m_pTransformCom->GetLocation() + MATH::RandVec() * MATH::RandReal({ 0.5f,1.f });
		_Particle.Name = L"Blood";
		_Particle.Speed = Speed;
		ParticleSystem::Instance().PushParticle(_Particle);
	}
	
	if (FloorBloodCurrentCoolTime < 0.f)
	{
		FloorBloodCurrentCoolTime = FloorBloodCoolTime;

		Sphere _Sphere = _CollisionComp->_Sphere;
		_Sphere.Radius *= 1.5f;
		{
			const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

			for (const auto& _CurPlane : _CurMap)
			{
				vec3 ToPlaneCenter = _CurPlane.Center - _Sphere.Center;
				const float Distance = MATH::Length(ToPlaneCenter);
				if (Distance > CCollisionComponent::MapCollisionCheckDistanceMin)continue;

				auto CheckInfo = Collision::IsPlaneToSphere(_CurPlane, _Sphere);
				const bool bCurCollision = CheckInfo.first;
				auto& CollisionInfo = CheckInfo.second;

				// 평면과는 일단 충돌한다.
				if (bCurCollision)
				{
					const vec3 ProjPt = MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
					// 평면과 일단 충돌한 상태에서
					// 구체의 중심을 평면에 투영한 이후의 점이 평면의 내부에 있다면 충돌.
					if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
					{
						CollisionInfo.IntersectPoint = ProjPt;
						FloorBlood(_CurPlane, CollisionInfo.IntersectPoint);
						continue;
					}
					// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
					std::array<Segment, 3ul> _Segments = MATH::MakeSegmentFromFace(_CurPlane.Face);
					for (const auto& _CurSegment : _Segments)
					{
						float t0 = 0;
						float t1 = 0;
						vec3 IntersectPoint;
						auto IsCollision = Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0, t1, IntersectPoint);
						if (IsCollision.first)
						{
							CollisionInfo.IntersectPoint = IntersectPoint;
							FloorBlood(_CurPlane, CollisionInfo.IntersectPoint);
							continue;
						}
					}
				}
			};
		}

		{
			const auto& _CurMap = CCollisionComponent::_MapFloorInfo;

			for (const auto& _CurPlane : _CurMap)
			{
				vec3 ToPlaneCenter = _CurPlane.Center - _Sphere.Center;
				const float Distance = MATH::Length(ToPlaneCenter);
				if (Distance > CCollisionComponent::MapCollisionCheckDistanceMin)continue;

				auto CheckInfo = Collision::IsPlaneToSphere(_CurPlane, _Sphere);
				const bool bCurCollision = CheckInfo.first;
				auto& CollisionInfo = CheckInfo.second;

				// 평면과는 일단 충돌한다.
				if (bCurCollision)
				{
					const vec3 ProjPt = MATH::ProjectionPointFromFace(_CurPlane._Plane, _Sphere.Center);
					// 평면과 일단 충돌한 상태에서
					// 구체의 중심을 평면에 투영한 이후의 점이 평면의 내부에 있다면 충돌.
					if (MATH::InnerPointFromFace(ProjPt, _CurPlane.Face))
					{
						CollisionInfo.IntersectPoint = ProjPt;
						FloorBlood(_CurPlane, CollisionInfo.IntersectPoint);
						continue;
					}
					// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
					std::array<Segment, 3ul> _Segments = MATH::MakeSegmentFromFace(_CurPlane.Face);
					for (const auto& _CurSegment : _Segments)
					{
						float t0 = 0;
						float t1 = 0;
						vec3 IntersectPoint;
						auto IsCollision = Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0, t1, IntersectPoint);
						if (IsCollision.first)
						{
							CollisionInfo.IntersectPoint = IntersectPoint;
							FloorBlood(_CurPlane, CollisionInfo.IntersectPoint);
							continue;
						}
					}
				}
			};
		}
	}
}

void CMonster::DeadProcess()
{

	for(const size_t GibIdx : GibTable)
	{
		CollisionParticle _CollisionParticle;
		_CollisionParticle.Delta = 10000.f;
		_CollisionParticle.bCollision = true;
		_CollisionParticle.bFloorCollision = true;
		_CollisionParticle.bWallCollision = false;
		_CollisionParticle.bMapBlock = true;
		_CollisionParticle.Gravity = MATH::RandReal({20.f,30.f});
		vec3 SpawnLocation = m_pTransformCom->GetLocation() + MATH::Normalize(MATH::RandVec()) * MATH::RandReal({0.f,2.f});
		_CollisionParticle.Scale = { 0.5f,0.5f,0.5f };
		_CollisionParticle.Location  = _CollisionParticle.StartLocation = SpawnLocation;
		_CollisionParticle.Dir = MATH::Normalize(vec3{ MATH::RandReal({-1,1}),0.f,MATH::RandReal({-1,1}) }); 
		_CollisionParticle.Angle = MATH::RandReal({ 90,130 });
		_CollisionParticle.Speed = MATH::RandReal({ 10,20});
		_CollisionParticle.Rotation = { 0.f,0.f,MATH::RandReal({-180,180}) };
		_CollisionParticle.Durtaion = 180.0f;
		_CollisionParticle.Name = L"Gib";
		_CollisionParticle.CurrentFrame = _CollisionParticle.EndFrame = GibIdx;
		_CollisionParticle.Radius = 0.3f;
		ParticleSystem::Instance().PushCollisionParticle(std::move(_CollisionParticle));
	}
}

void CMonster::Free()
{
	/// <summary> 2020 12 20 이호준
	 SafeRelease(_VertexBuffer);
	/// </summary>
	
	
	for (auto& rPair : m_mapTexture)	// map 텍스처 릴리즈
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	CGameObject::Free();
}
