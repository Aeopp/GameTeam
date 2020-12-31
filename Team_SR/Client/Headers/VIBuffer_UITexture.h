#pragma once
#ifndef __VIBUFFER_UITEXTURE_H__
#include "VIBuffer.h"
USING(Engine)
class CVIBuffer_UITexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_UITexture(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVIBuffer_UITexture() = default;

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void * pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	HRESULT SetDisUVposX(float _xUV);
	HRESULT ReverseSetDisUVposX(float _xUV);
	HRESULT SetDisUVposY(float _yUV);
	HRESULT ResetDisUVpos();

public:
	static CVIBuffer_UITexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	_vector m_vSize;
	_vector m_vCenter;
	D3DXVECTOR2 m_v2UVArr[4];
};

#define __VIBUFFER_UITEXTURE_H__
#endif