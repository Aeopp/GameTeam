#pragma once
#ifndef ParticleSystem_H
#define ParticleSystem_H 
#include "GameObject.h"
#include "DXWrapper.h"
#include "Vertexs.h"
#include "CollisionComponent.h"

USING(Engine)

struct Particle
{
	std::wstring Name = L"";
	vec3 Location = { 0,0,0 };
	vec3 Rotation = { 0,0,0 };
	vec3 Scale = { 1,1,1 };
	vec3 Dir = { 0,0,0 };
	vec3 DeltaVector = { 0,0,0 };
	float Angle = 0.0f;
	float Gravity = 0.0f;
	vec3 StartLocation{ 0,0,0 };
	
	bool bLoop = true;
	float Durtaion = 0.1f;
	float AlphaLerp = 1.f;
	float Speed = 10.f;
	float Delta = 0.01f;
	float CurrentT = 0.0f;
	uint32_t CurrentFrame = 0ul;
	// [begin,end)
	uint32_t EndFrame = 1ul;
	float T = 0.0f;
};

struct CollisionParticle : public Particle
{
	float Radius = 1.f;
	bool bMapBlock = false;
	bool bWallCollision = false;
	bool bFloorCollision = false;
	bool bCollision = false;
	// Lhs->Particle Tag            Rhs->Object Tag
	CCollisionComponent::ETag _Tag;
};

class ParticleSystem
{
private:
	ParticleSystem() = default;
public:
	friend class std::shared_ptr<ParticleSystem>;
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = delete;
	ParticleSystem& operator=(ParticleSystem&&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
public:
	static void Initialize(  CManagement* const _Management,IDirect3DDevice9* const _Device)noexcept;
	static void Release()noexcept;
	static FORCEINLINE ParticleSystem& Instance() { return *_Instance; };
private:
	static std::shared_ptr<ParticleSystem> _Instance;
private:
	IDirect3DDevice9* _Device;
	CManagement* _Management;
	std::vector<Particle> _Particles;
	std::vector<CollisionParticle> _CollisionParticles;
	AnimationTextures _ParticleTextureTable;
	std::shared_ptr<IDirect3DVertexBuffer9> _VertexBuf{ nullptr };
	std::shared_ptr<IDirect3DVertexDeclaration9> _VertexDecl{ nullptr };
	uint32_t VertexCount = 6ul;
	uint32_t TriangleCount = 2ul;
	uint16_t VertexByteSize = sizeof(Vertex::Texture);
private:
	void EventBind()&;
	void InitializeVertex()&noexcept;
	void InitializeTextures() & noexcept;
public:
	void Update(const float DeltaTime)&;
	void LateUpdate(const float DeltaTime)&;
	void Collision()&;
	void Render()&;
	void ClearParticle()& noexcept;
	void ParticleEventFromName(Particle& _Particle, const float DeltaTime);
	void ParticleRenderSetFromName( Particle& _Particle, Effect::Info& _Effect);
	void ParticleCollisionEventFromName(CollisionParticle& _Particle);
public:
	void PushParticle(Particle _Particle);
	void PushCollisionParticle(CollisionParticle _Particle);
};

#endif // ParticleSystem_H