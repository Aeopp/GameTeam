#pragma once
#ifndef __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT ReadyComponentPrototype() = 0;
	virtual HRESULT ReadyComponent(void* pArg = nullptr) = 0;
	virtual HRESULT Render_VIBuffer() = 0;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	/* 
	버텍스버퍼 컴객체: 여러 버텍스들을 배열 형식으로 보관하고 관리하는 컴객체.
	*/
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr; 
	_uint m_iVertexSize = 0;
	_uint m_iVertexCount = 0;
	_uint m_iTriCount = 0;
	_uint m_iFVF = 0; /* Flexible Vertex Format(유연한 정점 포맷) */

	/*
	인덱스버퍼 컴객체: 여러 인덱스들을 배열 형식으로 보관하고 관리하는 컴객체.
	*인덱스: 하나의 폴리곤을 구성하는 정점들의 이웃관계를 표현하는 도구.
	*/
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
	_uint		m_iIndexSize = 0;
	D3DFORMAT	m_IndexFormat;

	//void*	m_pVertices = nullptr;
	//_bool	m_IsClone = false;
};
END

#define __VIBUFFER_H__
#endif

