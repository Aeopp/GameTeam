#pragma once
#ifndef __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTexture : public CComponent
{
private:	
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice, ETextureType eType, TCHAR* pFilePath, _uint iCount = 1);
	explicit CTexture(const CTexture& other);
	virtual ~CTexture() = default;

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void * pArg = nullptr) override;

public:
	HRESULT Set_Texture(_uint iIndex);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, ETextureType eType, TCHAR* pFilePath, _uint iCount = 1);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	typedef vector<IDirect3DBaseTexture9*> TEXTURES;
	TEXTURES	m_Textures;

	TCHAR*	m_pFilePath = nullptr;
	_uint	m_iCount = 0;
	ETextureType	m_eType;
};
END

#define __TEXTURE_H__
#endif