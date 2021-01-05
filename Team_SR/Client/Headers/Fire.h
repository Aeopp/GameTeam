#pragma once
#ifndef Fire_h__

#include "Monster.h"
USING(Engine)
class CFire final : public CMonster
{
private:
	explicit CFire(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFire() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();
	bool	FrameMove_Fire(float fDeltaTime);
public:
	static CFire* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float m_fFrameSpeed = 0.f;


	// CMonster을(를) 통해 상속됨
	virtual void FreezeHit() override;

};

#define Fire_h__
#endif // Fire_h__