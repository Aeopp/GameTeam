#pragma once
#ifndef __MONSTER_H__

#include "GameObject.h"
USING(Engine)
class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
protected:
	virtual HRESULT AddComponents();
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	class CTexture* m_pTextureCom = nullptr;
	class CVIBuffer* m_pVIBufferCom = nullptr;
};

#define  __MONSTER_H__
#endif
