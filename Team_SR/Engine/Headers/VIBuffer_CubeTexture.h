#pragma once
#ifndef __VIBUFFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_CubeTexture final : public CVIBuffer
{
protected:
	explicit CVIBuffer_CubeTexture(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVIBuffer_CubeTexture() = default;

public:
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_CubeTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END

#define __VIBUFFER_CUBETEXTURE_H__
#endif

