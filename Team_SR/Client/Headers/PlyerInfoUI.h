#pragma once

#ifndef __PLAYERINFOUI_H__
#include "..\Headers\GameUI.h"

//UI는 Transform을 쓰지 말고 새로 할것


USING(Engine)
class CPlyerInfoUI final : public CGameUI
{
private:
	explicit CPlyerInfoUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlyerInfoUI() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:	//상호작용관련


private:	//내부메서드관련
	HRESULT	AddComponent();

public:
	static CPlyerInfoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:


};

#define __PLAYERINFOUI_H__
#endif // !__PLAYERINFOUI_H__
