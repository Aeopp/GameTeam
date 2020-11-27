#include "VIBuffer_RectTexture.h"

USING(Engine)

CVIBuffer_RectTexture::CVIBuffer_RectTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

HRESULT CVIBuffer_RectTexture::ReadyComponentPrototype()
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
	pVertex[0].vUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPosition = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVertex[1].vUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPosition = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVertex[2].vUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPosition = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVertex[3].vUV = D3DXVECTOR2(0.f, 1.f);

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

HRESULT CVIBuffer_RectTexture::ReadyComponent(void * pArg)
{
	if (FAILED(CVIBuffer::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectTexture::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 장치에게 인덱스버퍼 셋 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	/* 인덱스를 이용해서 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

CVIBuffer_RectTexture* CVIBuffer_RectTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_RectTexture");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectTexture::Clone(void * pArg)
{
	CVIBuffer_RectTexture* pClone = new CVIBuffer_RectTexture(*this);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectTexture");
		SafeRelease(pClone);
	}
	SafeAddRef(m_pDevice);

	return pClone;
}

void CVIBuffer_RectTexture::Free()
{
	CVIBuffer::Free();
}
