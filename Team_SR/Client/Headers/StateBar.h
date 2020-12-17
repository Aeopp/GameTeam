#pragma once
#include "..\Headers\Component.h"
#ifndef __STATEBAR_H__

USING(Engine)
class CStateBar final : public CComponent
{
private:
	explicit CStateBar(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStateBar() = default;

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void * pArg = nullptr) override;

public: //Static 함수
	static HRESULT test();

public:
	static CStateBar* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

};

#define __STATEBAR_H__
#endif // !__STATEBAR_H__