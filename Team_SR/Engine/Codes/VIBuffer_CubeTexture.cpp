#include "VIBuffer_CubeTexture.h"

USING(Engine)

CVIBuffer_CubeTexture::CVIBuffer_CubeTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

HRESULT CVIBuffer_CubeTexture::ReadyComponentPrototype()
{
	m_iVertexSize = sizeof(VTXCUBE_TEXTURE);
	m_iVertexCount = 8;
	m_iFVF = FVF_VTXCUBE_TEXTURE;
	m_iTriCount = 12;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::ReadyComponentPrototype()))
		return E_FAIL;

	VTXCUBE_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[0].vUV = pVertex[0].vPosition;

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].vUV = pVertex[1].vPosition;

	pVertex[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].vUV = pVertex[2].vPosition;

	pVertex[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].vUV = pVertex[3].vPosition;

	pVertex[4].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[4].vUV = pVertex[4].vPosition;

	pVertex[5].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].vUV = pVertex[5].vPosition;

	pVertex[6].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].vUV = pVertex[6].vPosition;

	pVertex[7].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].vUV = pVertex[7].vPosition;	

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 5;
	pIndex[8]._2 = 4;
	pIndex[8]._3 = 7;

	pIndex[9]._1 = 5;
	pIndex[9]._2 = 7;
	pIndex[9]._3 = 6;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::ReadyComponent(void * pArg)
{
	if (FAILED(CVIBuffer::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 장치에게 인덱스버퍼 셋 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	/* 인덱스를 이용해서 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

CVIBuffer_CubeTexture* CVIBuffer_CubeTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_CubeTexture* pInstance = new CVIBuffer_CubeTexture(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeTexture");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_CubeTexture::Clone(void * pArg)
{
	CVIBuffer_CubeTexture* pClone = new CVIBuffer_CubeTexture(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		SafeRelease(pClone);
	}

	return pClone;
}

void CVIBuffer_CubeTexture::Free()
{
	CVIBuffer::Free();
}
