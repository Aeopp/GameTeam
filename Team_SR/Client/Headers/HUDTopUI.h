#pragma once
#include "GameUI.h"
#ifndef __HUDBOSSBAR_H__

class CHUDTopUI : public CGameUI
{
private:
	explicit CHUDTopUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHUDTopUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:	//내부메서드관련
	HRESULT	AddComponent();

public:
	void SetMaxHPAndHP(int* _piMaxValue, int* _piValue);
public:
	static CHUDTopUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pBossHPVIBufferCom = nullptr;
	class CTexture* m_pBossHPTextureCom = nullptr;

	class CLoadingBar* m_pBossHPBar = nullptr;

private:
	UI_DESC m_tBossUIDesc;
	bool m_bShown;
#pragma region Test_Font
	int m_iMax;
	int m_iMin;
#pragma endregion
};

#define __HUDBOSSBAR_H__
#endif // !__HUDBOSSBAR_H__
