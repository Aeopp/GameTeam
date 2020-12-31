#pragma once
#include "GameUI.h"

USING(Engine)
class CShopUI :	public CGameUI
{
private:
	explicit CShopUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShopUI() = default;

public:
	// CGameUI��(��) ���� ��ӵ�
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:

private:
	HRESULT	AddComponent();

public:
	static CShopUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
protected:
	bool m_bShown;


};

