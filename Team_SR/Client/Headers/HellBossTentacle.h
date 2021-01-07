#pragma once
#ifndef __HELLBOSSTENTACLE_H__

#include "Bullet.h"
USING(Engine)
class CHellBossTentacle final : public CBullet
{
private:
	explicit CHellBossTentacle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHellBossTentacle() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동

private:
	virtual HRESULT AddComponents() override;

public:
	static CHellBossTentacle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:
	bool bRelayFlag;	// 전달 여부
};

#define __HELLBOSSTENTACLE_H__
#endif