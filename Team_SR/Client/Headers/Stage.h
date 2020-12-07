#pragma once
#ifndef __STAGE_H__

#include "Scene.h"

USING(Engine)
class CStage : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage() = default;

public:
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;

protected:
	virtual _uint KeyProcess(float fDeltaTime) override;

private:
	HRESULT AddPlayerLayer(const wstring& LayerTag);
	HRESULT AddTerrainLayer(const wstring& LayerTag);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

};

#define __STAGE_H__
#endif