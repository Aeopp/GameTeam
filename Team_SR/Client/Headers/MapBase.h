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
protected:
	void LoadMap(std::wstring FilePath,const mat& MapWorld);
	void LoadFloor(const std::wstring& FilePath );
	void LoadCubeMap(const std::wstring& FilePath);
	void LoadBars(const std::wstring& FilePath);
public:
	virtual void Free() override;
protected:
	std::shared_ptr<std::vector<SubSetInfo>> _WallSubSetInfo{};
	std::shared_ptr<std::vector<SubSetInfo>> _BarSubSetInfo{};
	std::shared_ptr<std::vector<SubSetInfo>> _FloorSubSetInfo{};
	std::shared_ptr<IDirect3DCubeTexture9>   _CubeTexture{};
	std::shared_ptr<IDirect3DVertexBuffer9>  _CubeVertexBuf{};
	std::shared_ptr<IDirect3DIndexBuffer9> _CubeIndexBuf{};
	mat MapWorld;
private:
	void CubeMapRender();
	void WallRender();
	void FloorRender();
	void BarRender();
};

#define __MapBase_H__
#endif