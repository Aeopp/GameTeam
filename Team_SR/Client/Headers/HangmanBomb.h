#pragma once
#ifndef __HangmanBomb_h__
#define __HangmanBomb_h__

#include "Bullet.h"
USING(Engine)
class CHangmanBomb final : public CBullet
{
private:
	explicit CHangmanBomb(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CHangmanBomb() = default;
public:
	// CBullet을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	virtual HRESULT AddComponents() override;
	void Movement(float fDeltaTime);
	void CreateFire();
public:
	static CHangmanBomb* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float m_fAngle = 0.f;
	float m_fStartY = 0.f;
	float m_fJumpPower = 0.f;
	float m_fJumpTime = 0.f;
};

#endif // __HangmanBomb_h__