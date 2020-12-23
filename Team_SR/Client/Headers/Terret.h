#pragma once
#ifndef Terret_h__
#define Terret_h__

#include "GameObject.h"
USING(Engine)
class CTerret final :	public CGameObject
{
private:
	explicit CTerret(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTerret() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	HRESULT AddComponents();
	void	FindTarget();
	void	Fire(float fDeltaTime);
	void	UpdateAngle();
	void	IsBillboarding();
public:
	static CTerret* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	CTexture* m_pTexture;
	float m_fFrameCnt;
	float m_fTestTime;
	float m_fAngle = 0.f;
	CGameObject* m_pTarget = nullptr;
	
};

#endif // Terret_h__