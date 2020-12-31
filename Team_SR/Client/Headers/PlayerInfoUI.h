#pragma once

#ifndef __PLAYERINFOUI_H__
#include "..\Headers\GameUI.h"

//UI�� Transform�� ���� ���� ���� �Ұ�


USING(Engine)
class CPlayerInfoUI final : public CGameUI
{
private:
	explicit CPlayerInfoUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayerInfoUI() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

public:	//��ȣ�ۿ����


private:	//���θ޼������
	HRESULT	AddComponent();

public:
	static CPlayerInfoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:

};

#define __PLAYERINFOUI_H__
#endif // !__PLAYERINFOUI_H__
