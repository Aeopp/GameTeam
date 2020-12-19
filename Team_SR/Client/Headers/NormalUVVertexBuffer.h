#pragma once

#ifndef NormalUVVertexBuffer_h__
#define NormalUVVertexBuffer_h__
#include "Component.h"
#include "Vertexs.h"
#include "DXWrapper.h"

USING(Engine)

class CNormalUVVertexBuffer : public CComponent
{
public:
	explicit CNormalUVVertexBuffer(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CComponent;
	virtual HRESULT ReadyComponentPrototype() override;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) override;
	virtual HRESULT Render();
public:
	static CNormalUVVertexBuffer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free() override;
	std::shared_ptr<IDirect3DVertexBuffer9> _VertexBuf{ nullptr };
private:
	uint32_t VertexCount = 6ul;
	uint32_t TriangleCount = 2ul;
	uint16_t VertexByteSize =   0ul ;
	std::shared_ptr<IDirect3DVertexDeclaration9> VertexDecl{ nullptr };
};

#endif // NormalUVVertexBuffer_h__