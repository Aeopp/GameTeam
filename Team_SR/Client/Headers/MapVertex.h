//#pragma once
//#ifndef __MAPVERTEX_H__
//#include "Component.h"
//#include <memory>
//
//USING(Engine)
//class CMapVertex abstract : public CComponent
//{
//protected:
//	explicit CMapVertex(LPDIRECT3DDEVICE9 pDevice,
//		std::wstring FilePath);
//	virtual ~CMapVertex() = default;
//public:
//	virtual HRESULT ReadyComponentPrototype() = 0;
//	virtual HRESULT ReadyComponent(void* pArg = nullptr) = 0;
//	virtual HRESULT Render_MapVertex() = 0;
//public:
//	virtual CComponent* Clone(void* pArg = nullptr) = 0;
//	virtual void Free() override;
//	using Super = CComponent;
//protected :
//
//	///* 
//	//���ؽ����� �İ�ü: ���� ���ؽ����� �迭 �������� �����ϰ� �����ϴ� �İ�ü.
//	//*/
//	//LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr; 
//	//_uint m_iVertexSize = 0;
//	//_uint m_iVertexCount = 0;
//	//_uint m_iTriCount = 0;
//	//_uint m_iFVF = 0; /* Flexible Vertex Format(������ ���� ����) */
//
//	///*
//	//�ε������� �İ�ü: ���� �ε������� �迭 �������� �����ϰ� �����ϴ� �İ�ü.
//	//*�ε���: �ϳ��� �������� �����ϴ� �������� �̿����踦 ǥ���ϴ� ����.
//	//*/
//	//LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
//	//_uint		m_iIndexSize = 0;
//	//D3DFORMAT	m_IndexFormat;
//
//	////void*	m_pVertices = nullptr;
//	////_bool	m_IsClone = false;
//};
//
//#define __MAPVERTEX_H__
//#endif
//
