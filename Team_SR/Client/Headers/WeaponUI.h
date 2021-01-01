#pragma once
#ifndef __WEAPONUI_H__
#include "GameUI.h"

class CWeaponUI final : public CGameUI
{
private:
	explicit CWeaponUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWeaponUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	HRESULT	AddComponent(wstring _PrototypeTag, wstring _ComponentTag);
	HRESULT RenderText();

public:
	static CWeaponUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	static _uint s_iCloneCount;
	static LPD3DXFONT s_pFont;
	_uint m_iID;
	wstring m_wsObjectName;
	bool m_bOnWeapon = false;
};

#define __WEAPONUI_H__
#endif // !__WEAPONUI_H__