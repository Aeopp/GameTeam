#pragma once

#ifndef __PLAYERINFOUI_H__
#include "..\Headers\GameObject.h"

USING(Engine)
class CPlyerInfoUI final : public CGameObject
{
private:
	explicit CPlyerInfoUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlyerInfoUI() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:	//내부메서드관련
	HRESULT	AddComponent();

public:
	static CPlyerInfoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:	//컴포넌트
	class CVIBuffer* m_pVIBufferCom = nullptr; 
	class CTexture* m_pTextureCom = nullptr;	//일단 나중에
private:
};

#define __PLAYERINFOUI_H__
#endif // !__PLAYERINFOUI_H__
