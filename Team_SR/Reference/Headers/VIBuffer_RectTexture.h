#pragma once
#ifndef __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_RectTexture final : public CVIBuffer
{
protected:
	explicit CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_RectTexture(const CVIBuffer_RectTexture& other);
	virtual ~CVIBuffer_RectTexture() = default;

public:
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_RectTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END

#define __VIBUFFER_RECTTEXTURE_H__
#endif

