#include "stdafx.h"
#include "..\Headers\VIBuffer_UITexture.h"


CVIBuffer_UITexture::CVIBuffer_UITexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
	//_vector m_vSize;
	//_vector m_vCenter;
	////D3DXVECTOR2 m_v2UIValue;

	m_vSize = _vector(2.f, 2.f, 0);
	m_vCenter = _vector(0.5f, 0.5f, 0);
	m_v2UVArr[0] = D3DXVECTOR2(0.f + 0.01f, 0.f + 0.01f);
	m_v2UVArr[1] = D3DXVECTOR2(1.f - 0.01f, 0.f + 0.01);
	m_v2UVArr[2] = D3DXVECTOR2(1.f - 0.01f, 1.f - 0.01f);
	m_v2UVArr[3] = D3DXVECTOR2(0.f + 0.01f, 1.f - 0.01f);

	////Test
	//m_v2UVArr[0] = D3DXVECTOR2(0.f + 0.01f, 0.f + 0.01f);
	//m_v2UVArr[1] = D3DXVECTOR2(1.f - 0.01f, 0.f + 0.01);
	//m_v2UVArr[2] = D3DXVECTOR2(1.f - 0.01f, 1.f - 0.01f);
	//m_v2UVArr[3] = D3DXVECTOR2(0.f + 0.01f, 1.f - 0.01f);
}


HRESULT CVIBuffer_UITexture::ReadyComponentPrototype()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iVertexCount = 4;
	m_iFVF = FVF_VTX_TEXTURE;
	m_iTriCount = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::ReadyComponentPrototype()))
		return E_FAIL;

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVertex[0].vUV = m_v2UVArr[0];
	pVertex[1].vPosition = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVertex[1].vUV = m_v2UVArr[1];
	pVertex[2].vPosition = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVertex[2].vUV = m_v2UVArr[2];
	pVertex[3].vPosition = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVertex[3].vUV = m_v2UVArr[3];

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_UITexture::ReadyComponent(void * pArg/* = nullptr*/)
{
	if (FAILED(CVIBuffer::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_UITexture::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 장치에게 인덱스버퍼 셋 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	/* 인덱스를 이용해서 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}


//HRESULT CVIBuffer_UITexture::SetDisposVertex()
//{
//	//x축 : 비율을 정해서 먼저 계산한 것을 -값 늦게 계산한 것을 +값으로 한다.
//	//y축 : 
//	VTX_TEXTURE* pVertex = nullptr;
//	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
//
//	_vector vBefor = { 0, 0, 0 };
//	vBefor.x = m_vSize.x * m_vCenter.x;
//	vBefor.y = m_vSize.y * m_vCenter.y;
//	_vector vAfter = { 0, 0, 0 };
//	vAfter.x = m_vSize.x - vBefor.x;
//	vAfter.y = m_vSize.y - vBefor.y;
//
//	pVertex[0].vPosition = D3DXVECTOR3(-vBefor.x, vAfter.y, 0.f);
//	pVertex[0].vUV = m_v2UVArr[0];
//	pVertex[1].vPosition = D3DXVECTOR3(vAfter.x, vAfter.y, 0.f);
//	pVertex[1].vUV = m_v2UVArr[1];
//	pVertex[2].vPosition = D3DXVECTOR3(vAfter.x, -vBefor.y, 0.f);
//	pVertex[2].vUV = m_v2UVArr[2];
//	pVertex[3].vPosition = D3DXVECTOR3(-vBefor.x, -vBefor.y, 0.f);
//	pVertex[3].vUV = m_v2UVArr[3];
//
//	m_pVB->Unlock();
//	return S_OK;
//}




HRESULT CVIBuffer_UITexture::SetDisUVposX(float _xUV)
{
	if (1.f <= _xUV)
		_xUV = 1.f - 0.01f;

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//pVertex[0].vUV = D3DXVECTOR2(0.f + 0.01f, 0.f + 0.01f);
	pVertex[1].vUV = D3DXVECTOR2(_xUV - 0.01f, 0.f + 0.01);
	pVertex[2].vUV = D3DXVECTOR2(_xUV - 0.01f, 1.f - 0.01f);
	//pVertex[3].vUV = D3DXVECTOR2(0.f + 0.01f, 1.f - 0.01f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_UITexture::ReverseSetDisUVposX(float _xUV)
{
	if (1.f <= _xUV)
		_xUV = 1.f - 0.01f;

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vUV = D3DXVECTOR2((1.f - _xUV) + 0.01f, 0.f + 0.01f);
	//pVertex[1].vUV = D3DXVECTOR2(1.f - 0.01f, 0.f + 0.01);
	//pVertex[2].vUV = D3DXVECTOR2(1.f - 0.01f, 1.f - 0.01f);
	pVertex[3].vUV = D3DXVECTOR2((1.f - _xUV) + 0.01f, 1.f - 0.01f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_UITexture::SetDisUVposY(float _yUV)
{


	return S_OK;
}

HRESULT CVIBuffer_UITexture::ResetDisUVpos()
{
	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vUV = m_v2UVArr[0];
	pVertex[1].vUV = m_v2UVArr[1];
	pVertex[2].vUV = m_v2UVArr[2];
	pVertex[3].vUV = m_v2UVArr[3];

	m_pVB->Unlock();

	return S_OK;
}

CVIBuffer_UITexture * CVIBuffer_UITexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_UITexture* pInstance = new CVIBuffer_UITexture(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_RectTexture");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_UITexture::Clone(void * pArg/* = nullptr*/)
{
	CVIBuffer_UITexture* pClone = new CVIBuffer_UITexture(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectTexture");
		SafeRelease(pClone);
	}

	return pClone;
}

void CVIBuffer_UITexture::Free()
{
	CVIBuffer::Free();
}
