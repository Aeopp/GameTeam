#pragma once
#ifndef __CAMERA_H__
#include "GameObject.h"
USING(Engine)
class CCamera :	public CGameObject
{
private:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCamera();

	// CGameObject��(��) ���� ��ӵ�
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
	//ī�޶� �ٶ󺸴¹���. == m_vecLook
	_vector m_vecDir;
	//target���� ���� ������ �Ÿ�.
	float	m_fDistance;
};
#define __CAMERA_H__
#endif

