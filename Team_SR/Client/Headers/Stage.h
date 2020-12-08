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
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	class CPlayer* m_pPlayer = nullptr;
	class CMainCamera* _Camera{ nullptr };
	class CMapBase* _CurrentMap{ nullptr };
};

#define __STAGE_H__
#endif
