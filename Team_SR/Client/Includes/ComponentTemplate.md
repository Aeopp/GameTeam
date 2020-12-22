
// 헤더 ...................
#pragma once

#ifndef NormalUVVertexBuffer_h__
#define NormalUVVertexBuffer_h__
#include "Component.h"
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
};

#endif // NormalUVVertexBuffer_h__



// 구현..............

#include "stdafx.h"
#include "..\Headers\NormalUVVertexBuffer.h"
#include "..\Headers\VIBuffer.h"

USING(Engine)

CNormalUVVertexBuffer::CNormalUVVertexBuffer(LPDIRECT3DDEVICE9 pDevice) :
	Super(pDevice ) 
{
}

HRESULT CNormalUVVertexBuffer::ReadyComponentPrototype()
{
Super::ReadyComponentPrototype();

	return S_OK;
}

HRESULT CNormalUVVertexBuffer::ReadyComponent(void* pArg)
{
Super::ReadyComponent(pArg);
	return S_OK;
}

HRESULT CNormalUVVertexBuffer::Render()
{
     Super::Render();

	return S_OK;
}

CNormalUVVertexBuffer* CNormalUVVertexBuffer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNormalUVVertexBuffer* pInstance = new CNormalUVVertexBuffer(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent* CNormalUVVertexBuffer::Clone(void* pArg)
{
	CNormalUVVertexBuffer* pClone = new CNormalUVVertexBuffer(*this);
	SafeAddRef(m_pDevice);
	////////////////
	이곳에서 참조카운팅 관리........
	////////////////
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(__FUNCTIONW__ , __FUNCTIONW__);
		SafeRelease(pClone);
	}

	return pClone;
}

void CNormalUVVertexBuffer::Free()
{
// ....... 이곳에 자원삭제
////////////

	Super::Free();
}
