#pragma once
#ifndef __LAYER_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
	typedef list<class CGameObject*>	GAMEOBJECTS;
public:
	class CGameObject* GetGameObject(_uint iIndex = 0);
	GAMEOBJECTS	 GetGameObjects();
public:
	HRESULT AddGameObjectInLayer(class CGameObject* pGameObject);
	_uint UpdateGameObject(float fDeltaTime);
	_uint LateUpdateGameObject(float fDeltaTime);
public:
	// 2020.12.16 11:50 KMJ
	// Remove 플래그가 ON된 게임 오브젝트 삭제
	void RemoveGameObject();

public:
	static CLayer* Create();
	virtual void Free() override;
	static const std::wstring Tag;
private:
	GAMEOBJECTS	m_GameObjects;	/* 게임 오브젝트의 클론들을 보관 */
};
END

#define __LAYER_H__
#endif