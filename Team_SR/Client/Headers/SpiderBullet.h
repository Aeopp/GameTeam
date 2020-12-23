#pragma once
#ifndef SpiderBullet_h__
#define SpiderBullet_h__

#include "Bullet.h"
USING(Engine)
class CSpiderBullet final : public CBullet
{
private:
	explicit CSpiderBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpiderBullet() = default;

public:
	// CBullet을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	virtual HRESULT AddComponents() override;
public:
	static CSpiderBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#endif // SpiderBullet_h__