#pragma once
#ifndef __COLLISIONMANAGER_H__

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollisionManager final : public CBase
{
	DECLARE_SINGLETON(CCollisionManager)
private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;
public:
	_uint Update(float fDeltaTime);
	_uint LateUpdate(float fDeltaTime);
public:
	virtual void Free() override;
};
END

#define __COLLISIONMANAGER_H__
#endif

