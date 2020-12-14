#pragma once
#ifndef __COLLISIONMANAGER_H__

#include "Base.h"
#include "GameObject.h"
#include "Collision.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionManager final : public CBase
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
	void ClearComponent()&noexcept;
public:
	std::vector<PlaneInfo> _PlaneInfo;
private:
	std::vector<class CCollisionComponent*> _CollisionComponents;
};
END

#define __COLLISIONMANAGER_H__
#endif

