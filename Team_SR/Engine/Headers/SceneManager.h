#pragma once
#ifndef __SCENEMANAGER_H__

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)
class CSceneManager final : public CBase
{
	DECLARE_SINGLETON(CSceneManager)

private:
	CSceneManager();
	virtual ~CSceneManager()= default;

public:
	HRESULT SetUpCurrentScene(_int iSceneID, CScene* pCurrentScene);
	_uint UpdateScene(float fDeltaTime);
	_uint LateUpdateScene();
	FORCEINLINE CScene* GetCurrentScene() { return m_pCurrentScene; };
public:
	virtual void Free() override;

private:
	CScene*	m_pCurrentScene = nullptr;
	_int  m_iSceneID = -1;
};
END

#define __SCENEMANAGER_H__
#endif
