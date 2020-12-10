#include "..\Headers\CollisionManager.h"
#include "Layer.h"
#include "CollisionComponent.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{

}

_uint CCollisionManager::Update(float fDeltaTime)
{
	// ¸Ê°ú Ãæµ¹ ...
	for (auto& _CollisionComp : _CollisionComponents)
	{
		if (!_CollisionComp->bCollision)continue;
		_CollisionComp->bMapBlock;


	}
	return _uint();
}

_uint CCollisionManager::LateUpdate(float fDeltaTime)
{
	return _uint();
}

void CCollisionManager::Free()
{
	ClearComponent();
}

void CCollisionManager::ClearComponent() & noexcept
{
	std::for_each(std::begin(_CollisionComponents),
		std::end(_CollisionComponents), 
		[](CCollisionComponent* _CCollisionComponent)
		{
			SafeRelease(_CCollisionComponent);
		});
	
	_CollisionComponents.clear();
	_CollisionComponents.shrink_to_fit();

	_PlaneInfo.clear();
	_PlaneInfo.shrink_to_fit();

}
