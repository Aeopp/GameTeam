#pragma once
#ifndef __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CComponent : public CBase
{
public:
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CComponent() = default;

public:
	virtual HRESULT ReadyComponentPrototype() = 0;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) = 0;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9	m_pDevice;
};
END

#define __COMPONENT_H__
#endif