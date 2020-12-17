#pragma once
#ifndef __GLACIERBULLET_H__
#include "Bullet.h"
USING(Engine)
class CGlacierBullet :	public CBullet
{
private:
	explicit CGlacierBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGlacierBullet() = default;

	// CBullet을(를) 통해 상속됨
public:
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	virtual HRESULT AddComponents() override;
public:
	static CGlacierBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	
};
#define __GLACIERBULLET_H__
#endif

