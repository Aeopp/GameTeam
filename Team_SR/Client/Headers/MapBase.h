#pragma once
#ifndef __MapBase_H__

#include "GameObject.h"
#include "DXWrapper.h"
#include "Vertexs.h"

USING(Engine)
class CMapBase  abstract : public CGameObject
{
protected:
	explicit CMapBase (LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMapBase () = default;
public:
	using Super = CGameObject;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
	const std::vector<PlaneInfo>& RefMapPolygonPlane();
protected:
	void LoadMap(std::wstring FilePath,const mat& MapWorld);
public:
	virtual void Free() override;
protected:
	std::shared_ptr<std::vector<SubSetInfo>> _SubsetInfo;
	std::shared_ptr<std::vector<PlaneInfo>> _PolygonPlane;
	mat MapWorld;
	// REMOVEPLZ
	vec4 diffusecolor = { 1.f,0.f,0.f,1.f };
public:
};

#define __MapBase_H__
#endif