#pragma once
#ifndef __GAMEOBJECT_H__

#include "Base.h"
#include "Collision.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGameObject() = default;

public:
	class CComponent* GetComponent(const wstring & ComponentTag);

public:
	virtual HRESULT ReadyGameObjectPrototype() = 0;	/* ������Ÿ�� �ʱ�ȭ */
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0; /* Ŭ�� �ʱ�ȭ */
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
	//            ������Ʈ�� ���� ������Ʈ ���� �浹 �Ǹ� ȣ��. 
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo);
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo);
	static const std::wstring Tag;
	class CTransform* GetTransform() { return m_pTransformCom; };
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	HRESULT AddComponent(
		_int iSceneIndex,
		const wstring & PrototypeTag,
		const wstring & ComponentTag,
		class CComponent** ppComponent,
		void* pArg = nullptr);

	virtual HRESULT AddStaticComponents();
protected:
	LPDIRECT3DDEVICE9	m_pDevice;

	typedef unordered_map<wstring, class CComponent*> COMPONENTS;
	COMPONENTS	m_Components;

	//���������� �ʿ��� ������Ʈ��
protected:
	class CTransform* m_pTransformCom = nullptr;
	class CManagement* m_pManagement = nullptr;
};
END

#define __GAMEOBJECT_H__
#endif