#pragma once
#ifndef __GAMEOBJECTMANAGER_H__

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameObjectManager final : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)

private:
	CGameObjectManager();
	virtual ~CGameObjectManager() = default;

public:
	CGameObject* GetGameObject(_int iSceneIndex, const wstring& LayerTag, _uint iIndex = 0);
	std::list<class CGameObject*> GetGameObjects(_int iSceneIndex, const wstring& LayerTag);
public:
	HRESULT ReserveSizeContainer(_int iSceneCount);
	HRESULT AddGameObjectPrototype(_int iSceneIndex,  wstring GameObjectTag, CGameObject* pPrototype);
	HRESULT AddGameObjectInLayer(_int iFromSceneIndex, const wstring& GameObjectTag, _int iToSceneIndex, const wstring& LayerTag, CGameObject** ppGameObject = nullptr, void* pArg = nullptr);
	HRESULT ClearForScene(_int iSceneIndex);
	HRESULT ClearForSceneClone(const _int iSceneIndex)&;
public:
	_uint UpdateGameObject(float fDeltaTime);
	_uint LateUpdateGameObject(float fDeltaTime);

public:
	// 2020.12.16 11:50 KMJ
	// Remove 플래그가 ON된 게임 오브젝트 삭제
	void RemoveGameObject();

private:
	CGameObject* CloneGameObjectPrototype(_int iSceneIndex, const wstring& GameObjectTag, void* pArg = nullptr);

public:
	virtual void Free() override;

private:
	typedef unordered_map<wstring, CGameObject*> PROTOTYPES;
	PROTOTYPES*	m_pPrototypes = nullptr;

	typedef unordered_map<wstring, class CLayer*>	LAYERS;
	LAYERS*	m_pLayers = nullptr;

	_int m_iSceneCount = 0;
	
};
END

#define __GAMEOBJECTMANAGER_H__
#endif

