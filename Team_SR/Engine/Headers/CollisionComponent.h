#pragma once
#ifndef __COLLISIONCOMPONENT_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionComponent final : public CComponent
{
protected:
	explicit CCollisionComponent(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCollisionComponent() = default;
public:
	using Super = CComponent;
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
public:
	static CCollisionComponent* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END

#define __COLLISIONCOMPONENT_H__
#endif

