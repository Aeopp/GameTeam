#pragma once
#ifndef SharkBullet_h__
#define SharkBullet_h__


#include "Monster.h"
USING(Engine)
class CSharkBullet final : public CMonster
{
private:
	explicit CSharkBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSharkBullet() = default;

public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo);
private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();
public:
	static CSharkBullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	wstring m_wstrBase;
};


#endif // SharkBullet_h__