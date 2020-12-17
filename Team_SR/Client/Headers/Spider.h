#pragma once
#ifndef Spider_h__
#define Spider_h__

#include "Monster.h"
USING(Engine)
class CSpider final : public CMonster
{
private:
	explicit CSpider(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpider() = default;

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
	//void AI_SecondPhase();
	void AI_DeadPhase();
	////////////////////ACTION/////////////////////
	bool Action_Move(float fDeltaTime);
	bool Action_Idle(float fDeltaTime);
	bool Action_Shoot(float fDeltaTime);
	bool Action_Death(float fDeltaTime);
	///////////////////////////////////////////////
	//void CreateBullet();

private:
	enum class AWARENESS { No, Yes, End };
	enum class PHASE { HP_High, HP_ZERO, End };

	using AIFunc = void(CSpider::*)(void);
	using ACTFunc = bool(CSpider::*)(float);

private:
	virtual HRESULT AddComponents() override;
	HRESULT Set_Texture();

public:
	static CSpider* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	////////////////////////////////////////////////////
	AWARENESS	m_eAwareness = AWARENESS::End;
	PHASE		m_ePhase = PHASE::End;
	AIFunc		m_fpSpiderAI[(_uint)AWARENESS::End][(_uint)PHASE::End];
	ACTFunc		m_fpAction;
};

#endif // Spider_h__