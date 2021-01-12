#pragma once
#include "Scene.h"

USING(Engine)
class CTitle : public CScene
{
public:
	explicit CTitle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTitle() = default;

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;

private:

public:
	_uint KeyProcess(float fDeltaTime);

public:
	static CTitle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	ESceneID CurrentSceneID;
	ESceneID NextSceneID;

	class CButtonUI* m_pButtonUI = nullptr;
	class CSceneImage* m_pTitleImage = nullptr;
};

