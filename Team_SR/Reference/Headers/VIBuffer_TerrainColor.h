#pragma once
#ifndef __VIBUFFER_TERRAINCOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_TerrainColor final : public CVIBuffer
{
protected:
	explicit CVIBuffer_TerrainColor(LPDIRECT3DDEVICE9 pDevice, 
		_uint iVertexCountX,
		_uint iVertexCountZ,
		float fVertexInterval = 1.f);
	explicit CVIBuffer_TerrainColor(const CVIBuffer_TerrainColor& other);
	virtual ~CVIBuffer_TerrainColor() = default;

public:
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_TerrainColor* Create(LPDIRECT3DDEVICE9 pDevice, _uint iVertexCountX, _uint iVertexCountZ, float fVertexInterval = 1.f);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	_uint m_iVertexCountX = 0;
	_uint m_iVertexCountZ = 0;
	float m_fVertexInterval = 1.f;
};
END

#define __VIBUFFER_TERRAINCOLOR_H__
#endif

