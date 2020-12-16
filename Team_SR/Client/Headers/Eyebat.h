#pragma once
#ifndef Eyebat_h__
#define Eyebat_h__

#include "Monster.h"
USING(Engine)
class CEyebat final : public CMonster
{
private:
	explicit CEyebat(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEyebat() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	void Update_AI(float fDeltaTime);

	////////////////////AWARENESS//////////////////
	void AI_NoAwareness();
	void AI_FirstPhase();
	void AI_SecondPhase();
	void AI_DeadPhase();
	////////////////////ACTION/////////////////////
	bool Action_Move(float fDeltaTime);
	bool Action_Idle(float fDeltaTime);
	bool Action_Shoot(float fDeltaTime);
	bool Action_Death(float fDeltaTime);
	///////////////////////////////////////////////
	void CreateBullet();

private:
	enum class AWARENESS { No, Yes, End };
	enum class PHASE { HP_High, HP_Low, HP_ZERO, End };

	using AIFunc = void(CEyebat::*)(void);
	using ACTFunc = bool(CEyebat::*)(float);

private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();

public:
	static CEyebat* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float m_fCountDown = 0.f;
	wstring m_wstrBase;

	////////////////////////////////////////////////////
	AWARENESS	m_eAwareness = AWARENESS::End;
	PHASE		m_ePhase = PHASE::End;
	AIFunc		m_fpEyebatAI[(_uint)AWARENESS::End][(_uint)PHASE::End];
	ACTFunc		m_fpAction;

};

#endif // Eyebat_h__