#pragma once
#ifndef __WEAPONUI_H__
#include "GameUI.h"

class CWeaponUI final : public CGameUI
{
private:
	explicit CWeaponUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWeaponUI() = default;

public:
	// CGameUI��(��) ���� ��ӵ�
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	void SetShownUI() { m_bShown = true; m_fShownTime = 0.f; }

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
	bool m_bOnWeapon = true;

	float m_fShownTime = 0.f;
	const float m_cfMaxShownTime;
};

#define __WEAPONUI_H__
#endif // !__WEAPONUI_H__