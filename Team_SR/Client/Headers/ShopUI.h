#pragma once
#include "GameUI.h"

USING(Engine)
class CShopUI :	public CGameUI
{
private:
	explicit CShopUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShopUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:
	void SetShownBarUI() { m_bShown = true; }//ui나타내기
	void SetInvisibleBarUI() { m_bShown = false; }//ui숨기기

private:
	HRESULT	AddComponent();

public:
	static CShopUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
protected:
	bool m_bShown;


};

