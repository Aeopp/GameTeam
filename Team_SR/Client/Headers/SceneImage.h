#pragma once
#include "GameUI.h"

USING(Engine)
class CSceneImage : public CGameUI
{
public:
	explicit CSceneImage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSceneImage() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	HRESULT AddComponent(wstring _PrototypeTag, wstring _ComponentTag);

public:
	static CSceneImage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:

};

