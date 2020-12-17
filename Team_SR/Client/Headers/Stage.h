#pragma once
#ifndef __STAGE_H__

#include "Scene.h"

USING(Engine)
class CStage abstract : public CScene
{
protected:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage() = default;
public:
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;
protected:
	virtual _uint KeyProcess(float fDeltaTime) override;
	virtual void PlayerKeyProcess(class CPlayer*const _CurrentPlayer,float fDeltaTime);
public:
	virtual void Free() override;
public:
	class CPlayer* m_pPlayer = nullptr;
	class CMainCamera* _Camera{ nullptr };
	class CMapBase* _CurrentMap{ nullptr };
	class CUIManager* m_pUIManager;
};

#define __STAGE_H__
#endif
