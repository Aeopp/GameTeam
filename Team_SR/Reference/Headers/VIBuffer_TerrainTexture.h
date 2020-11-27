#pragma once
#ifndef __VIBUFFER_TERRAINTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_TerrainTexture final : public CVIBuffer
{
protected:
	explicit CVIBuffer_TerrainTexture(LPDIRECT3DDEVICE9 pDevice,
		_uint iVertexCountX,
		_uint iVertexCountZ,
		float fVertexInterval = 1.f);
	explicit CVIBuffer_TerrainTexture(const CVIBuffer_TerrainTexture& other);
	virtual ~CVIBuffer_TerrainTexture() = default;

public:
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_TerrainTexture* Create(LPDIRECT3DDEVICE9 pDevice, _uint iVertexCountX, _uint iVertexCountZ, float fVertexInterval = 1.f);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	_uint m_iVertexCountX = 0;
	_uint m_iVertexCountZ = 0;
	float m_fVertexInterval = 1.f;
};
END

#define __VIBUFFER_TERRAINTEXTURE_H__
#endif

