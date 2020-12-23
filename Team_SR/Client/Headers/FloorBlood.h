#pragma once
#ifndef FloorBlood_h__
#define FloorBlood_h__


#include "Effect.h"
class CFloorBlood final : public CEffect
{
private:
	explicit CFloorBlood(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFloorBlood() = default;

public:
	// CEffect을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo) override;
private:
	virtual HRESULT AddComponents() override;
	/////////////////////////////////////////////
private:
	void Jump(float fDeltaTime);
public:
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CFloorBlood* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	bool	m_bTest = false;
	float m_fStartY = 0.f;
	float m_fJumpPower = 0.f;
	float m_fJumpTime = 0.f;
};


#endif // FloorBlood_h__