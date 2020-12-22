#pragma once
#ifndef __HUD_HPBAR_H__

#include "GameUI.h"

USING(Engine)
class CHUD_HpBar final : public CGameUI
{
private:
	explicit CHUD_HpBar(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHUD_HpBar() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	void SetMaxHPAndHP(int& _iMaxHP, int& _iHP);

protected://내부메서드관련
	HRESULT	AddComponent();

public:
	static CHUD_HpBar* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float m_fRatio = 1.f;//Test
	const float m_fMaxSize;
};

#define __HUD_HPBAR_H__
#endif // !__HUD_HPBAR_H__