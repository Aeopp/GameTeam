#pragma once
#ifndef __MAP1ST_H__

#include "MapBase.h"

USING(Engine)
class CMap1st final : public CMapBase
{
private:
	explicit CMap1st(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMap1st() = default;
public:
	using Super = CMapBase;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	/*  Ex) ..\\Resources\\Map\\0\\ */
	static CMap1st* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

};

#define __MAP1ST_H__
#endif