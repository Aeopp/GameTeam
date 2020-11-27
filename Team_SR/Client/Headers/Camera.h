#pragma once
#ifndef __CAMERA_H__
#include "GameObject.h"
USING(Engine)
class CCamera :	public CGameObject
{
private:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCamera();

	// CGameObject을(를) 통해 상속됨
public:
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
public:
	HRESULT SetViewMatrix(TRANSFORM_DESC vPos);
private:
	HRESULT AddComponent();
private:
	//카메라가 바라보는방향. == m_vecLook
	_vector m_vecDir;
	//target으로 부터 떨어진 거리.
	float	m_fDistance;
};
#define __CAMERA_H__
#endif

