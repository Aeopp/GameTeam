#pragma once
#ifndef __GAMEOBJECT_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject(const CGameObject& other);
	virtual ~CGameObject() = default;	

public:
	class CComponent* GetComponent(const wstring& ComponentTag);

public:
	virtual HRESULT ReadyGameObjectPrototype() = 0;	/* 프로토타입 초기화 */
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0; /* 클론 초기화 */
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	HRESULT AddComponent(
		_int iSceneIndex, 
		const wstring& PrototypeTag, 
		const wstring& ComponentTag,
		class CComponent** ppComponent, 
		void* pArg = nullptr);

protected:
	LPDIRECT3DDEVICE9	m_pDevice;

	typedef unordered_map<wstring, class CComponent*> COMPONENTS;
	COMPONENTS	m_Components;
};
END

#define __GAMEOBJECT_H__
#endif