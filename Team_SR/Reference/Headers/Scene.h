#pragma once
#ifndef __SCENE_H__

#include "Base.h"
#include "KeyMgr.h"
BEGIN(Engine)
class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

public:
	virtual HRESULT ReadyScene() = 0;
	virtual _uint UpdateScene(float fDeltaTime) = 0;
	virtual _uint LateUpdateScene() = 0;

protected:
	virtual _uint KeyProcess(float fDeltaTime);
public:
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9		m_pDevice;
	CKeyMgr*				m_pKeyMgr;
	class CManagement*		m_pManagement;
};
END

#define __SCENE_H__
#endif