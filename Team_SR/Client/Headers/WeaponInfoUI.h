#pragma once
#include "GameUI.h"
class CWeaponInfoUI final : public CGameUI
{
private:
	explicit CWeaponInfoUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWeaponInfoUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:


public:
	static CWeaponInfoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;


private:

};

