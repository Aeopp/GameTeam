#pragma once
#ifndef __SCENE_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

public:
	virtual HRESULT ReadyScene() = 0;
	virtual _uint UpdateScene() = 0;
	virtual _uint LateUpdateScene() = 0;

public:
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9	m_pDevice;
};
END

#define __SCENE_H__
#endif