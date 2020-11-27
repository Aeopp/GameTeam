#pragma once
#ifndef __RENDERER_H__

#include "Base.h"

BEGIN(Engine)

/* 렌더링 행위를 그룹화로 나누는 역할을 수행 */
class CRenderer final : public CBase
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRenderer() = default;

public:
	HRESULT AddGameObjectInRenderer(ERenderID eID, class CGameObject* pGameObject);
	HRESULT Render(HWND hWnd = nullptr);

private:
	HRESULT RenderPriority();
	HRESULT RenderNoAlpha();
	HRESULT RenderAlpha();
	HRESULT RenderUI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	LPDIRECT3DDEVICE9	m_pDevice;

	typedef list<class CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects[(_uint)ERenderID::MaxCount];
};
END

#define __RENDERER_H__
#endif