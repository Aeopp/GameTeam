#pragma once
#ifndef __CMiniMap_H__

#include "Bullet.h"
#include "DXWrapper.h"


USING(Engine)
class CMiniMap final : public CGameObject
{
private:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMiniMap() = default;
public:
	using Super = CGameObject;
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
	
public:
	static CMiniMap* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	static const vec2 MiniMapModeFirstScreenOffset;
	static const vec2 MiniMapModeSecondScreenOffset;
	vec3 MiniMapScale{ 1,1,1 };
	uint32_t MiniMapVertexCount;
	uint32_t MiniMapTriangleCount;
	uint16_t MiniMapVertexByteSize;
	std::shared_ptr<IDirect3DVertexBuffer9> MiniMapVertexBuf;
	std::shared_ptr<IDirect3DVertexDeclaration9> MiniMapVertexDecl;

	uint32_t PlayerMarkerVertexCount;
	uint32_t PlayerMarkerTriangleCount;
	uint16_t PlayerMarkerVertexByteSize;
	std::shared_ptr<IDirect3DTexture9> PlayerMarkerTexture;
	std::shared_ptr<IDirect3DVertexBuffer9> PlayerMarkerVertexBuf;
	std::shared_ptr<IDirect3DVertexDeclaration9> PlayerMarkerVertexDecl;
public:
	struct InitializeInfo 
	{
		std::vector<vec3> _Points;
		mat MapWorld;
	};
private:
	struct MiniRenderInfo
	{
		vec3 Location;
		float Rotation;
		vec4 ColorCoefft;
	};
	void MiniMapRenderModeFirst(mat MiniMapProjection);
	void MiniMapRenderModeSecond(mat MiniMapProjection);
	void RenderInformationsFirst(const int32_t RenderFlag,const float MiniMapScaleCorrection, const vec2 OffSetXY, const vec3 RenderScale, std::vector<MiniRenderInfo>& _TargetLocations, typename Effect::Info& _CurrentRenderEffect)&;
	void RenderInformationsSecond(const float WorldRelativeRotation,const vec3 WorldRelativeLocation,const int32_t RenderFlag, const float MiniMapScaleCorrection, const vec2 OffSetXY, const vec3 RenderScale, std::vector<MiniRenderInfo>& _TargetLocations, typename Effect::Info& _CurrentRenderEffect)&;
	std::vector<MiniRenderInfo>  _MonsterLocations;
	std::vector<MiniRenderInfo>    _ItemLocations;
};

#define __CMiniMap_H__
#endif