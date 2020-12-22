#pragma once
#include "MapBase.h"

USING(Engine)
class CMap6th final : public CMapBase
{
private:
	explicit CMap6th(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMap6th() = default;
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
	static CMap6th* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};