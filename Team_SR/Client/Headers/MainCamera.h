#pragma once
#ifndef __MAINCAMERA_H__

#include "Camera.h"

USING(Engine)
class CMainCamera  final : public CCamera
{
private:
	explicit CMainCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMainCamera() = default;
public:
	using Super = CCamera;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
private:

};

#define __MAINCAMERA_H__
#endif