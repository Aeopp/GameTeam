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
	void SetMaxHPAndHP(float* _pfMaxValue, float* _pfValue);
public:
	static CHUDTopUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pBossHPVIBufferCom = nullptr;
	class CTexture* m_pBossHPTextureCom = nullptr;

	class CLoadingBar* m_pMonsterHPBar = nullptr;

private:
	UI_DESC m_tBossUIDesc;
	bool m_bMonsterHPbarShown = false;

};

#define __HUDBOSSBAR_H__
#endif // !__HUDBOSSBAR_H__
