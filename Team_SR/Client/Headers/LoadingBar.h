#pragma once
#ifndef __LOADINGBAR_H__

#include "GameUI.h"

USING(Engine)
class CLoadingBar final : public CGameUI
{
private:
	explicit CLoadingBar(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoadingBar() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	void SetMaxValueAndMinValue(_int* _piMaxValue, _int* _piValue);

	int GetMaxValue();
	int	GetMinValue();


private://내부메서드관련
	HRESULT	AddComponent(wstring _PrototypeTag, wstring _ComponentTag);
	HRESULT RenderText();

public:
	static CLoadingBar* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;


	float m_fRatio = 1.f;//Test
	float m_fMaxSize;
	bool m_bTextOut;
	wstring m_wsObjectName;

	_int* m_piMaxValue;
	_int* m_piMinValue;
};

#define __LOADINGBAR_H__
#endif // !__LOADINGBAR_H__