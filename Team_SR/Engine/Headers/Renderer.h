#pragma once
#ifndef __RENDERER_H__

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRenderer() = default;

public:
	HRESULT AddGameObjectInRenderer(ERenderID eID, class CGameObject* pGameObject);
	HRESULT Render(HWND hWnd = nullptr);
	void RegistLight(const D3DLIGHT9& Light);
	void SetAmbient(const DWORD Ambient) { this->Ambient = Ambient; };
private:
	HRESULT RenderPriority();
	HRESULT RenderNoAlpha();
	HRESULT RenderAlpha();
	HRESULT RenderUI();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
	D3DCAPS9 _Caps9;
	size_t MaxTexState = 8;
private:
	DWORD Ambient = 0x00202020;
	LPDIRECT3DDEVICE9	m_pDevice;
	std::vector<D3DLIGHT9> _Lights;
	typedef list<class CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects[(_uint)ERenderID::MaxCount];
};
END

#define __RENDERER_H__
#endif