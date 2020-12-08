#pragma once
#ifndef __MapBase_H__

#include "GameObject.h"

USING(Engine)
class CMapBase  final : public CGameObject
{
private:
	explicit CMapBase (LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMapBase () = default;
public:
	using Super = CGameObject;
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CMapBase * Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};

#define __MapBase_H__
#endif