#include "stdafx.h"
#include "..\Headers\ParticleSystem.h"
#include "Camera.h"
#include "Player.h"
#include "MainCamera.h"
#include "SoundMgr.h"

std::shared_ptr<ParticleSystem>ParticleSystem::_Instance{nullptr};

void ParticleSystem::Initialize(CManagement* const _Management,IDirect3DDevice9* const _Device) noexcept
{
	if (!_Instance)
	{
		_Instance = std::shared_ptr<ParticleSystem>(new ParticleSystem{});
		_Instance->_Device = _Device;
		_Instance->_Management = _Management;
		_Instance->EventBind();
		_Instance->InitializeVertex();
		_Instance->InitializeTextures();
	};
}

void ParticleSystem::Release() noexcept
{
	_Instance->_ParticleTextureTable.Release();
	_Instance.reset();
}

void ParticleSystem::EventBind()&
{
	_Management->_ParticleCollision = [this]()
	{
		Collision();
	};

	_Management->_ParticleUpdate = [this](const float DeltaTime)
	{
		Update(DeltaTime);
	};

	_Management->_ParticleLateUpdate= [this](const float DeltaTime)
	{
		LateUpdate(DeltaTime);
	};
	
	_Management->SetEffectRender([this]() {Render();  });
}

void ParticleSystem::InitializeVertex() & noexcept
{
	std::vector<Vertex::Texture> _Vertexs(6);

	for (size_t i = 0; i < VertexCount; ++i)
	{
		_Vertexs[i].Normal = { 0.f,0.f,-1.f };
	}

	_Vertexs[0].TexCoord = { 0.03f,0.97f };
	_Vertexs[0].Location = { -0.5f,-0.5f,0.f };
	_Vertexs[1].TexCoord = { 0.03f,0.03f };
	_Vertexs[1].Location = { -0.5f,+0.5f,0.f };
	_Vertexs[2].TexCoord = { 0.97f,0.03f };
	_Vertexs[2].Location = { +0.5f,+0.5f,0.f };
	_Vertexs[3].TexCoord = { 0.03f,0.97f };
	_Vertexs[3].Location = { -0.5f,-0.5f,0.f };
	_Vertexs[4].TexCoord = { 0.97f,0.03f };
	_Vertexs[4].Location = { +0.5f,+0.5f,0.f };
	_Vertexs[5].TexCoord = { 0.97f,0.97f };
	_Vertexs[5].Location = { +0.5f,-0.5f,0.f };

	CreateVertex(_Device, _Vertexs, VertexCount, TriangleCount, VertexByteSize, _VertexBuf, _VertexDecl);
	Vertex::Texture* _VertexPtr = nullptr;
	_VertexBuf->Lock(0, 0, (void**)&_VertexPtr, 0);

	for (size_t i = 0; i < VertexCount; i += 3)
	{
		const auto Tangent_BiNormal = Mesh::CalculateTangentBinormal(
			TempVertexType{ _VertexPtr[i].Location,  _VertexPtr[i].TexCoord },
			TempVertexType{ _VertexPtr[i + 1].Location,_VertexPtr[i + 1].TexCoord },
			TempVertexType{ _VertexPtr[i + 2].Location,_VertexPtr[i + 2].TexCoord });

		const vec3 Normal = Mesh::CalculateNormal(Tangent_BiNormal.first, Tangent_BiNormal.second);

		for (size_t j = i; j <= (i + 2); ++j)
		{
			_VertexPtr[j].Normal = Normal;
			_VertexPtr[j].Tangent = Tangent_BiNormal.first;
			_VertexPtr[j].BiNormal = Tangent_BiNormal.second;
		};
	};


}

void ParticleSystem::InitializeTextures() & noexcept
{
	_ParticleTextureTable._TextureMap[L"OverKill"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\OverKill\\", 18);

	_ParticleTextureTable._TextureMap[L"BulletShell"] =
		CreateTexturesSpecularNormal(_Device, L"..\\Resources\\Effect\\BulletShell\\", 1);

	_ParticleTextureTable._TextureMap[L"ShotGunShell"] =
		CreateTexturesSpecularNormal(_Device, L"..\\Resources\\Effect\\ShotGunShell\\", 1);

	_ParticleTextureTable._TextureMap[L"DaggerThrow"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\DaggerThrow\\", 1);

	_ParticleTextureTable._TextureMap[L"DaggerThrowParticle"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\DaggerThrowParticle\\", 1);

	_ParticleTextureTable._TextureMap[L"MagnumShell"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\MagnumShell\\", 1);
	
	_ParticleTextureTable._TextureMap[L"BulletHole0"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHole\\0\\", 1);

	_ParticleTextureTable._TextureMap[L"BulletHole1"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHole\\1\\", 1);

	_ParticleTextureTable._TextureMap[L"BulletHole2"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHole\\2\\", 1);

	_ParticleTextureTable._TextureMap[L"BulletHole3"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHole\\3\\", 1);

	_ParticleTextureTable._TextureMap[L"ArrowBack"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ArrowBack\\", 1);

	_ParticleTextureTable._TextureMap[L"ArrowX"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ArrowX\\", 1);

	_ParticleTextureTable._TextureMap[L"Blood"] = CreateTexturesSpecularNormal(
			_Device, L"..\\Resources\\Effect\\Blood\\", 1);

	_ParticleTextureTable._TextureMap[L"BloodBigHit1"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BloodBigHit1\\", 8);

	_ParticleTextureTable._TextureMap[L"BloodBigHit2"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BloodBigHit2\\", 8);

	_ParticleTextureTable._TextureMap[L"FloorBlood"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\FloorBlood\\", 4);

	_ParticleTextureTable._TextureMap[L"BulletHit0"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHit0\\", 4);

	_ParticleTextureTable._TextureMap[L"BulletHit1"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHit1\\", 4);

	_ParticleTextureTable._TextureMap[L"BulletHit2"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHit2\\", 4);

	_ParticleTextureTable._TextureMap[L"BulletHit3"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\BulletHit3\\", 4);

	_ParticleTextureTable._TextureMap[L"MagicBoom"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\MagicBoom\\", 7);

	_ParticleTextureTable._TextureMap[L"MagicBlast"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\MagicBlast\\", 7);

	_ParticleTextureTable._TextureMap[L"WandProjectile"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\WandProjectile\\", 7);

	_ParticleTextureTable._TextureMap[L"StaffFire"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\StaffFire\\", 1);

	_ParticleTextureTable._TextureMap[L"Explosion0"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Explosion0\\", 13);

	_ParticleTextureTable._TextureMap[L"Explosion1"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Explosion1\\", 13);

	_ParticleTextureTable._TextureMap[L"Explosion2"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Explosion2\\", 13);

	_ParticleTextureTable._TextureMap[L"Dynamite"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Dynamite\\", 1);

	_ParticleTextureTable._TextureMap[L"ElectricBeam"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ElectricBeam\\", 1);

	_ParticleTextureTable._TextureMap[L"ElectricLight"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ElectricLight\\", 9ul);

	_ParticleTextureTable._TextureMap[L"ElectricMedium"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ElectricMedium\\", 9ul);

	_ParticleTextureTable._TextureMap[L"ElectricHeavy"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ElectricHeavy\\", 9ul);	

	_ParticleTextureTable._TextureMap[L"ElectricBlast"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\ElectricBlast\\", 12ul);

	_ParticleTextureTable._TextureMap[L"Particle"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Particle\\", 1ul);

	_ParticleTextureTable._TextureMap[L"Snow"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Snow\\", 1ul);

	_ParticleTextureTable._TextureMap[L"Gib"] = CreateTexturesSpecularNormal(
		_Device, L"..\\Resources\\Effect\\Gib\\", 52ul);
}

void ParticleSystem::Update(const float DeltaTime)&
{
	for (size_t i = 0; i < _Particles.size(); ++i)
	{
		auto& _Particle = _Particles[i];

		_Particle.T += DeltaTime;

		_Particle.Durtaion -= DeltaTime;

		if (_Particle.Durtaion > 0.f)
		{
			_Particle.CurrentT += DeltaTime;
			if (_Particle.CurrentT > _Particle.Delta)
			{
				_Particle.CurrentT -= _Particle.Delta;
				++_Particle.CurrentFrame;

				if (_Particle.CurrentFrame >= _Particle.EndFrame)
				{
					if (_Particle.bLoop)
					{
						_Particle.CurrentFrame = 0ul;
					}
					else
					{
						_Particle.CurrentFrame = (_Particle.EndFrame - 1);
					}
				}
			}
			_Particle.Dir += _Particle.DeltaVector *DeltaTime;
			_Particle.Dir = MATH::Normalize(_Particle.Dir);
			if (_Particle.bMove)
			{
				_Particle.Location += _Particle.Dir * DeltaTime * _Particle.Speed;
			}
			
		//	_Particle.Location.y = MATH::Parabolic(_Particle.StartLocation.y, _Particle.Speed, _Particle.Angle, _Particle.T, _Particle.Gravity);

			ParticleEventFromName(_Particle, DeltaTime);
		}
		else
		{
			std::swap(_Particle, _Particles.back());
			_Particles.pop_back();
		}
	}

	for (size_t i = 0; i < _CollisionParticles.size(); ++i)
	{
		auto& _Particle = _CollisionParticles[i];

		_Particle.Durtaion -= DeltaTime;
		_Particle.T += DeltaTime;

		if (_Particle.Durtaion > 0.f)
		{
			_Particle.CurrentT += DeltaTime;
			if (_Particle.CurrentT > _Particle.Delta)
			{
				_Particle.CurrentT -= _Particle.Delta;
				++_Particle.CurrentFrame;

				if (_Particle.CurrentFrame >= _Particle.EndFrame)
				{
					if (_Particle.bLoop)
					{
						_Particle.CurrentFrame = 0ul;
					}
					else
					{
						_Particle.CurrentFrame = (_Particle.EndFrame - 1);
					}
				}
			}
			_Particle.Dir += _Particle.DeltaVector * DeltaTime;
			_Particle.Dir = MATH::Normalize(_Particle.Dir);
			if (_Particle.bMove)
			{
				_Particle.Location += _Particle.Dir * DeltaTime * _Particle.Speed;
			}
		//	_Particle.Location.y = MATH::Parabolic(_Particle.StartLocation.y, _Particle.Speed, _Particle.Angle, _Particle.T, _Particle.Gravity);

			ParticleEventFromName(_Particle, DeltaTime);
		}
		else
		{
			std::swap(_Particle, _CollisionParticles.back());
			_CollisionParticles.pop_back();
		}
	}
}


void ParticleSystem::LateUpdate(const float DeltaTime)&
{

}

void ParticleSystem::Collision()&
{
	for (auto&  _Particle : _CollisionParticles)
	{
		if (!_Particle.bCollision)
		{
			continue;
		}
		
		Sphere _Sphere;
		_Sphere.Center = _Particle.Location + _Particle.Correction;
		_Sphere.Radius = _Particle.Radius;

		for (auto& _Comp : CCollisionComponent::_Comps)
		{
			if (!_Comp->bCollision)continue;
#pragma region MatchingCheck
			auto iter = CCollisionComponent::_TagBind.find(_Particle._Tag);
			if (iter == std::end(CCollisionComponent::_TagBind))continue;
			if (iter->second.find(_Comp->_Tag) == std::end(iter->second))continue;
			vec3 ToRhs = _Sphere.Center - _Comp->_Sphere.Center;
#pragma endregion
			// 거리검사
			if (MATH::Length(ToRhs) > CCollisionComponent::CollisionCheckDistanceMin)continue;
			// ....
			auto IsCollision = Collision::IsSphereToSphere(_Sphere, _Comp->_Sphere);
			// 충돌함.
			if (IsCollision.first)
			{
				ParticleCollisionEventFromName(_Particle);
				_Comp->Owner->ParticleHit(&_Particle, IsCollision.second);
				/*_LhsOwner->Hit(_RhsOwner, IsCollision.second);
				_RhsOwner->Hit(_LhsOwner, IsCollision.second);*/
				//PRINT_LOG(L"충돌체끼리 충돌!!", L"충돌체끼리 충돌!!");
			}
			else
			{
				// PRINT_LOG(L"충돌체끼리 충돌하지않음!!", L"충돌체끼리 충돌하지않음!!");
			}
		}

		const auto& _CurMap = CCollisionComponent::_MapPlaneInfo;

		if (_Particle.bWallCollision)
		{
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
						CollisionInfo.Flag = L"Wall";
						CollisionInfo.IntersectPoint = ProjPt;
				
						if (_Particle.bMapBlock)
						{
							_Particle.Location += CollisionInfo.Dir * CollisionInfo.CrossValue;
						}
						ParticleCollisionEventFromName(_Particle);
						break;
					}
					else
					{
						// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
						bool bSegmentCollision = false;
						std::array<Segment, 3ul> _Segments = MATH::MakeSegmentFromFace(_CurPlane.Face);
						for (const auto& _CurSegment : _Segments)
						{
							float t0 = 0;
							float t1 = 0;
							vec3 IntersectPoint;
							auto IsCollision = Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0, t1, IntersectPoint);
							if (IsCollision.first)
							{
								CollisionInfo.Flag = L"Wall";
								CollisionInfo.IntersectPoint = IntersectPoint;
							
								if (_Particle.bMapBlock)
								{
									_Particle.Location += CollisionInfo.Dir * CollisionInfo.CrossValue;
								}
								ParticleCollisionEventFromName(_Particle);
								bSegmentCollision = true;
								break;
							}
						}
						if (bSegmentCollision)
							break;
					}
				}
			}
		}

		{
			const auto& _CurMapFloor = CCollisionComponent::_MapFloorInfo;

			if (_Particle.bFloorCollision)
			{
				for (const auto& _CurPlane : _CurMapFloor)
				{
					// 벽과의 충돌의 차이점은 바닥은 현재 최적화로
				   // 삼각형 두개 만으로 만들었기 때문에 중심점이 매우 멀수 있어서
				   // 거리검사로 컬링을 하지 않는다.
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
							CollisionInfo.Flag = L"Floor";
							CollisionInfo.IntersectPoint = ProjPt;
						
							if (_Particle.bMapBlock)
							{
								_Particle.Location += CollisionInfo.Dir * CollisionInfo.CrossValue;
							}
							ParticleCollisionEventFromName(_Particle);
							continue;
						}
						// 삼각형으로 선분 3개를 정의한 이후에 선분과 구의 충돌을 검사한다.
						std::array<Segment, 3ul> _Segments = MATH::MakeSegmentFromFace(_CurPlane.Face);
						for (const auto& _CurSegment : _Segments)
						{
							vec3 IntersectPoint;
							float t0 = 0;
							float t1 = 0;

							auto IsCollision = Collision::IsSegmentToSphere(_CurSegment, _Sphere, t0, t1, IntersectPoint);
							if (IsCollision.first)
							{
								CollisionInfo.Flag = L"Floor";
								CollisionInfo.IntersectPoint = IntersectPoint;
								if (_Particle.bMapBlock)
								{
									_Particle.Location += CollisionInfo.Dir * CollisionInfo.CrossValue;
								}
								ParticleCollisionEventFromName(_Particle);
								continue;
							}
						}
					}
				}
			}
		}

		// 충돌체 끼리의 충돌
		
	}
}

void ParticleSystem::Render()&
{
	_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE );
	_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	auto& _Effect = Effect::GetEffectFromName(L"DiffuseSpecular");
	_Device->SetVertexShader(_Effect.VsShader);
	_Device->SetPixelShader(_Effect.PsShader);

	mat View;
	_Device->GetTransform(D3DTS_VIEW, &View);
	View._43=View._42=View._41 = 0;
	View = MATH::Inverse(View);

	for (auto& _Particle : _Particles)
	{
		mat _ParticleWorld;
		mat _ParticleRotationMatrix;

		mat Scale, Rotation, RotX, RotY, RotZ, Translation;
		D3DXMatrixScaling(&Scale, _Particle.Scale.x, _Particle.Scale.y, _Particle.Scale.z);
		D3DXMatrixTranslation(&Translation, _Particle.Location.x, _Particle.Location.y, _Particle.Location.z);

		if (_Particle.bRotationMatrix)
		{
			_ParticleRotationMatrix = _Particle.RotationMatrix;
		}
		else
		{
			mat RotX, RotY, RotZ ;
			D3DXMatrixRotationX(&RotX, MATH::ToRadian(_Particle.Rotation.x ) );
			D3DXMatrixRotationY(&RotY, MATH::ToRadian(_Particle.Rotation.y));
			D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(_Particle.Rotation.z));
			_ParticleRotationMatrix = RotX * RotY * RotZ;
		}

		if (_Particle.bBillboard)
		{
			_ParticleWorld = Scale * _ParticleRotationMatrix * View * Translation;
		}
		else
		{
			_ParticleWorld = Scale * _ParticleRotationMatrix  * Translation;
		}

		_Effect.SetVSConstantData(_Device, "World", _ParticleWorld);

		const auto& TextureTuple =_ParticleTextureTable.GetTexture(_Particle.Name, _Particle.CurrentFrame);

		_Device->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), std::get<0>(TextureTuple));
		//_Device->SetTexture(_Effect.GetTexIdx("SpecularSampler"), std::get<1>(TextureTuple));
		//_Device->SetTexture(_Effect.GetTexIdx("NormalSampler"), std::get<2>(TextureTuple));

		_Effect.SetPSConstantData(_Device, "bSpecularSamplerBind", 0);
		_Effect.SetPSConstantData(_Device, "bNormalSamplerBind", 0);
		_Effect.SetPSConstantData(_Device, "Shine", 20.f);

		_Device->SetStreamSource(0, _VertexBuf.get(), 0, VertexByteSize);
		_Device->SetVertexDeclaration(_VertexDecl.get());
		_Effect.SetPSConstantData(_Device, "bUVAlphaLerp", _Particle.UVAlphaLerp);
		_Effect.SetPSConstantData(_Device, "LightCalcFlag", _Particle.LightCalcFlag);
		ParticleRenderSetFromName(_Particle, _Effect);
		_Effect.SetPSConstantData(_Device, "bUVAlphaLerp", 0l);
		_Effect.SetPSConstantData(_Device, "LightCalcFlag", 0l);
	}

	for (auto& _Particle : _CollisionParticles)
	{
		mat _ParticleWorld;
		mat _ParticleRotationMatrix;

		mat Scale, Rotation, RotX, RotY, RotZ, Translation;
		D3DXMatrixScaling(&Scale, _Particle.Scale.x, _Particle.Scale.y, _Particle.Scale.z);
		D3DXMatrixTranslation(&Translation, _Particle.Location.x, _Particle.Location.y, _Particle.Location.z);

		if (_Particle.bRotationMatrix)
		{
			_ParticleRotationMatrix = _Particle.RotationMatrix;
		}
		else
		{
			mat RotX, RotY, RotZ;
			D3DXMatrixRotationX(&RotX, MATH::ToRadian(_Particle.Rotation.x));
			D3DXMatrixRotationY(&RotY, MATH::ToRadian(_Particle.Rotation.y));
			D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(_Particle.Rotation.z));
			_ParticleRotationMatrix = RotX * RotY * RotZ;
		}

		if (_Particle.bBillboard)
		{
			_ParticleWorld = Scale * _ParticleRotationMatrix * View * Translation;
		}
		else
		{
			_ParticleWorld = Scale * _ParticleRotationMatrix * Translation;
		}

		_Effect.SetVSConstantData(_Device, "World", _ParticleWorld);

		const auto& TextureTuple = _ParticleTextureTable.GetTexture(_Particle.Name, _Particle.CurrentFrame);

		_Device->SetTexture(_Effect.GetTexIdx("DiffuseSampler"), std::get<0>(TextureTuple));
	/*	_Device->SetTexture(_Effect.GetTexIdx("SpecularSampler"), std::get<1>(TextureTuple));
		_Device->SetTexture(_Effect.GetTexIdx("NormalSampler"), std::get<2>(TextureTuple));*/

		_Effect.SetPSConstantData(_Device, "bSpecularSamplerBind", 0);
		_Effect.SetPSConstantData(_Device, "bNormalSamplerBind", 0);
		_Effect.SetPSConstantData(_Device, "Shine", 20.f);

		_Device->SetStreamSource(0, _VertexBuf.get(), 0, VertexByteSize);
		_Device->SetVertexDeclaration(_VertexDecl.get());
		_Effect.SetPSConstantData(_Device, "bUVAlphaLerp", _Particle.UVAlphaLerp);
		ParticleRenderSetFromName(_Particle, _Effect);
		_Effect.SetPSConstantData(_Device, "bUVAlphaLerp", 0l);
	}


	_Effect.SetPSConstantData(_Device, "bUVAlphaLerp", 0l);
	_Effect.SetPSConstantData(_Device, "LightCalcFlag", 0l);
	_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

void ParticleSystem::ClearParticle() & noexcept
{
	_Particles.clear();
	_CollisionParticles.clear();
}

void ParticleSystem::ParticleEventFromName(Particle& _Particle,
	const float DeltaTime)
{
	if (_Particle.Name.find(L"Explosion") != std::wstring::npos)
	{
		const float Factor = _Particle.MaxDuration / _Particle.Durtaion;
		const float DiffuseScale = 100.f / Factor;
		MyLight _Light;
		_Light.Diffuse = { DiffuseScale ,DiffuseScale ,DiffuseScale ,1 };
		_Light.Location = MATH::ConvertVec4(vec3{ _Particle.Location }, 1.f);
		_Light.Priority = 0ul;
		_Light.Radius = 10000.f  / Factor;
		Effect::RegistLight(std::move(_Light));
	}

	if (_Particle.Name == L"DaggerThrow")
	{
		CollisionParticle& _CollisionParticle = static_cast<CollisionParticle&>(_Particle);
		float ScaleFactor = _CollisionParticle.bCollision ? 2.f : -2.f;
		_CollisionParticle.Scale.x +=  (_CollisionParticle.Speed*DeltaTime*ScaleFactor);
		if (_CollisionParticle.Scale.x < 0.0f)
		{
			_CollisionParticle.Scale.x = 0.0f;
		}
		if (_CollisionParticle.bCollision)
		{
			_CollisionParticle.Correction = _CollisionParticle.Dir * (_CollisionParticle.Scale.x / 2.f);
		};
		//if (!_CollisionParticle.bCollision)
		{
			MyLight _Light;
			_Light.Diffuse = { 2,0,2,1 };
			_Light.Location = MATH::ConvertVec4(vec3{ _CollisionParticle.Correction + _CollisionParticle.Location }, 1.f);
			_Light.Priority = 0ul;
			_Light.Radius = 20.f;
			Effect::RegistLight(std::move(_Light));
		}
	}

	if (_Particle.Name == L"StaffFire")
	{
		CollisionParticle& _CollisionParticle = static_cast<CollisionParticle&>(_Particle);
		float ScaleFactor = _CollisionParticle.bCollision ? 3.f : -3.f;
		_CollisionParticle.Scale.x += (_CollisionParticle.Speed * DeltaTime * ScaleFactor);

		if (_CollisionParticle.Scale.x < 0.0f)
		{
			_CollisionParticle.Scale.x = 0.0f;
		}
		if (_CollisionParticle.bCollision)
		{
			_CollisionParticle.Correction = _CollisionParticle.Dir * (_CollisionParticle.Scale.x / 2.f);
		}
		

		//if (!_CollisionParticle.bCollision)
		{
			MyLight _Light;
			_Light.Diffuse = { 0,0,(_CollisionParticle.CurrentAttack / 15.0f),1 };
			_Light.Location = MATH::ConvertVec4(vec3{ _CollisionParticle.Correction + _CollisionParticle.Location }, 1.f);
			_Light.Priority = 0ul;
			_Light.Radius = (_CollisionParticle.CurrentAttack / 15.0f) * 20.f;
			Effect::RegistLight(std::move(_Light));
		}
	}

	if (_Particle.Name == L"BulletShell" || 
		_Particle.Name == L"ShotGunShell" || 
		_Particle.Name == L"MagnumShell" ||
		_Particle.Name == L"Blood" ||  
		_Particle.Name ==L"Dynamite" || 
		_Particle.Name == L"Gib")
	{
		if (_Particle.bLoop)
		{		
			_Particle.Location.y =
				_Particle.StartLocation.y +
				((_Particle.Speed *
					std::sinf(MATH::ToRadian(_Particle.Angle)) * _Particle.T)
					* -((_Particle.T * _Particle.T) * _Particle.Gravity * (1.f / 2.f)));
		}
	}
}

void ParticleSystem::ParticleRenderSetFromName( Particle& _Particle,Effect::Info& _Effect)
{
	if (_Particle.Name == L"DaggerThrow")
	{
		_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);	
		return;
	}
	if (_Particle.Name == L"DaggerThrowParticle")
	{
		_Effect.SetPSConstantData(_Device, "ColorLerpT", (1.0f - (_Particle.Durtaion / _Particle.MaxDuration)));
		_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		_Effect.SetPSConstantData(_Device, "ColorLerpT", 0.0f);
		return;
	}
	if(_Particle.Name == L"BulletHole0" || 
		_Particle.Name == L"BulletHole1" || 
		_Particle.Name == L"BulletHole2" || 
		_Particle.Name == L"BulletHole3" || 
		_Particle.Name == L"FloorBlood")
	{
		_Device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		_Device->SetRenderState(D3DRS_STENCILREF, 0x1);
		_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMPFUNC::D3DCMP_EQUAL);
		_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
		_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		_Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		return;
	}

	if (_Particle.Name.find(L"Electric") !=std::wstring::npos)
	{
		_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		return;
	}

	_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
}

void ParticleSystem::ParticleCollisionEventFromName(CollisionParticle& _Particle)
{
	if (_Particle.Name == L"BulletShell"  || 
		_Particle.Name == L"ShotGunShell" || 
		_Particle.Name == L"MagnumShell"  )
	{
		_Particle.Dir = { 0,0,0 };
		_Particle.bBillboard = true;
		_Particle.bLoop = false;
		_Particle.bRotationMatrix = true;
		mat RotX, RotY, RotZ, Rot;
		D3DXMatrixRotationX(&RotX, MATH::ToRadian(_Particle.Rotation.x));
		D3DXMatrixRotationY(&RotY, MATH::ToRadian(_Particle.Rotation.y));
		D3DXMatrixRotationZ(&RotZ, MATH::ToRadian(_Particle.Rotation.z));
		Rot = RotX * RotY * RotZ;
		D3DXMatrixRotationX(&RotX, MATH::ToRadian(90.f));
		_Particle.RotationMatrix = Rot; *RotX;
		_Particle.bCollision = false;
		_Particle.bMove = false;
		_Particle.bFloorCollision = false;
	};

	if (_Particle.Name == L"Gib")
	{
		_Particle.bMove = false;
		_Particle.bLoop = false;
		_Particle.bCollision = false;
		_Particle.bWallCollision = false;
		_Particle.bFloorCollision = false;
		_Particle.bBillboard = true;
		_Particle.Rotation.z = MATH::RandReal({ -180,+180 });
	};

	if (_Particle.Name == L"Dynamite")
	{
		_Particle.Dir = { 0,0,0 };
		_Particle.bBillboard = true;
		_Particle.bLoop = false;
		_Particle.bCollision = false;
		_Particle.bMove = false;
		_Particle.bWallCollision = false;
		_Particle.bFloorCollision = false;
		_Particle.Durtaion = 0.0f;

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_WEAPON);
		CSoundMgr::Get_Instance()->PlaySound(L"explosion_01.wav", CSoundMgr::CHANNELID::PLAYER_WEAPON);

		 //  여기에 폭발 로직을 추가 한다 .
		auto _Camera = dynamic_cast<CMainCamera*>(_Management->GetGameObject(-1, L"Layer_MainCamera", 0));
		_Camera->Shake(2.0f, MATH::RandVec(), 0.8f);

		Particle _DynamiteExplosion;
		_DynamiteExplosion.bBillboard = true;
		_DynamiteExplosion.bMove = false;
		_DynamiteExplosion.Delta = 0.10f;
		_DynamiteExplosion.EndFrame = 13ul;
		_DynamiteExplosion.MaxDuration = 		_DynamiteExplosion.Durtaion = _DynamiteExplosion.Delta * _DynamiteExplosion.EndFrame;
		_DynamiteExplosion.Location = _Particle.Location;
		_DynamiteExplosion.Name = L"Explosion" + std::to_wstring(MATH::RandInt({ 0 ,2 })); 
		static constexpr float ExplosionScale = 20.f;
		_DynamiteExplosion.Scale = { ExplosionScale ,ExplosionScale ,ExplosionScale };
		ParticleSystem::PushParticle(_DynamiteExplosion); 
		
		Sphere _Sphere;
		_Sphere.Center = _Particle.Location + _Particle.Correction;
		_Sphere.Radius = ExplosionScale;

			for (auto& _Comp : CCollisionComponent::_Comps)
			{
				if (!_Comp->bCollision)continue;
#pragma region MatchingCheck
				auto iter = CCollisionComponent::_TagBind.find(_Particle._Tag);
				if (iter == std::end(CCollisionComponent::_TagBind))continue;
				if (iter->second.find(_Comp->_Tag) == std::end(iter->second))continue;
				vec3 ToRhs = _Sphere.Center - _Comp->_Sphere.Center;
#pragma endregion
				// 거리검사
				if (MATH::Length(ToRhs) > CCollisionComponent::CollisionCheckDistanceMin)continue;
				// ....
				auto IsCollision = Collision::IsSphereToSphere(_Sphere, _Comp->_Sphere);
				// 충돌함.
				if (IsCollision.first)
				{
					_Comp->Owner->ParticleHit(&_Particle, IsCollision.second);
					/*_LhsOwner->Hit(_RhsOwner, IsCollision.second);
					_RhsOwner->Hit(_LhsOwner, IsCollision.second);*/
					//PRINT_LOG(L"충돌체끼리 충돌!!", L"충돌체끼리 충돌!!");
				}

			}
	};

	if (_Particle.Name == L"DaggerThrow")
	{
		static constexpr float Duration = 5.0f;
		_Particle.bWallCollision = false;
		_Particle.bFloorCollision = false;
		_Particle.bCollision = false;
		_Particle.Durtaion = Duration;
		_Particle.bMove = false;

		for (size_t i = 0; i < 40; ++i)
		{
			Particle _SpawnParticle;
			_SpawnParticle.Scale = { 0.20f,0.20f,0.20f };
			_SpawnParticle.bBillboard = true;
			_SpawnParticle.bLoop = true;
			_SpawnParticle.bRotationMatrix = false;
			_SpawnParticle.UVAlphaLerp = 0ul;
			_SpawnParticle.Delta = FLT_MAX;
			const vec3 Dir = MATH::RandVec();
			const float Speed = MATH::RandReal({ 3.3f,5.5f });  
			_SpawnParticle.Dir = Dir;
			_SpawnParticle.MaxDuration= 			_SpawnParticle.Durtaion = Duration;
			_SpawnParticle.Location = _Particle.Location  + 
				_Particle.Correction /*+ -_Particle.Dir * (_Particle.Speed *0.1f) + Dir * Speed*/;
			_SpawnParticle.Name = L"DaggerThrowParticle";
			_SpawnParticle.Speed = Speed; 
			_SpawnParticle.StartLocation = _SpawnParticle.Location;
			ParticleSystem::PushParticle(_SpawnParticle);
		}

		Particle _MagicBoom;
		_MagicBoom.Scale = { 1.f,1.f,1.f };
		_MagicBoom.bBillboard = true;
		_MagicBoom.bLoop = false;
		_MagicBoom.bRotationMatrix = false;
		_MagicBoom.UVAlphaLerp = 0ul;
		_MagicBoom.Delta = 0.1f;
		_MagicBoom.bMove = false;
		_MagicBoom.Durtaion = _MagicBoom.Delta * 7.0f;
		_MagicBoom.EndFrame = 7ul;
		_MagicBoom.Name = L"MagicBoom";
		_MagicBoom.Location = _Particle.Location + _Particle.Correction;
		_MagicBoom.StartLocation = _MagicBoom.Location;
		ParticleSystem::PushParticle(_MagicBoom);

		//CPlayer* _Player = dynamic_cast<CPlayer*>(CManagement::Get_Instance()->GetGameObject(-1, L"Layer_Player", 0));
		//_Player->CurrentAttack = 15.f;
		//_Target->Hit(_Player, _CollisionInfo);
	}

	if (_Particle.Name == L"StaffFire")
	{
		static constexpr float Duration = 5.0f;
		_Particle.bWallCollision = false;
		_Particle.bFloorCollision = false;
		_Particle.bCollision = false;
		_Particle.Durtaion = Duration;
		_Particle.bMove = false;

		for (size_t i = 0; i < (_Particle.CurrentAttack / 15.0f) * 13; ++i)
		{
			Particle _SpawnParticle;
			_SpawnParticle.Scale = { 0.40f,0.40f,0.40f };
			_SpawnParticle.bBillboard = true;
			_SpawnParticle.bLoop = true;
			_SpawnParticle.bRotationMatrix = false;
			_SpawnParticle.UVAlphaLerp = 0ul;
			_SpawnParticle.Delta = 0.12f;
			_SpawnParticle.EndFrame = 7ul;
			const vec3 Dir = MATH::RandVec();
			const float Speed = MATH::RandReal({ 1.8f,2.7f });
			_SpawnParticle.Dir = Dir;
			_SpawnParticle.MaxDuration = _SpawnParticle.Durtaion = Duration;
			_SpawnParticle.Location = _Particle.Location +
				_Particle.Correction /*+ -_Particle.Dir * (_Particle.Speed * 0.01f) + Dir * Speed*/;
			_SpawnParticle.Name = L"MagicBlast";
			_SpawnParticle.Speed = Speed;
			_SpawnParticle.StartLocation = _SpawnParticle.Location;
			ParticleSystem::PushParticle(_SpawnParticle);
		}

		Particle _MagicBoom;
		_MagicBoom.Scale = { 1.f,1.f,1.f };
		_MagicBoom.bBillboard = true;
		_MagicBoom.bLoop = false;
		_MagicBoom.bRotationMatrix = false;
		_MagicBoom.UVAlphaLerp = 0ul;
		_MagicBoom.Delta = 0.1f;
		_MagicBoom.bMove = false;
		_MagicBoom.Durtaion = _MagicBoom.Delta * 7.0f;
		_MagicBoom.EndFrame = 7ul;

		_MagicBoom.Name = L"MagicBoom";
		_MagicBoom.Location = _Particle.Location + _Particle.Correction;
		_MagicBoom.StartLocation = _MagicBoom.Location;
		ParticleSystem::PushParticle(_MagicBoom);
	}
}


std::pair<bool,Particle*> ParticleSystem::GetParticle(const std::wstring& Name)
{
	auto iter = std::find_if(std::begin(_Particles), std::end(_Particles), [Name](const auto& _CurParticle)
		{
			return _CurParticle.Name == Name;
		});

	if (iter != std::end(_Particles))
	{
		return  std::pair<bool, Particle*> {true,&(*iter)};
	}

	return {false,nullptr};
}

void ParticleSystem::PushParticle(const Particle& _Particle)
{
	static_assert(std::is_same<Particle, std::decay_t<decltype(_Particle)>>::value,"");

	_Particles.emplace_back((_Particle));
}

void ParticleSystem::PushCollisionParticle(const CollisionParticle& _Particle)
{
	static_assert(std::is_same<CollisionParticle, std::decay_t<decltype(_Particle)>>::value, "");

	_CollisionParticles.emplace_back((_Particle));
}


