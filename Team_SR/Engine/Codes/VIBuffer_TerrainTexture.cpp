#include "VIBuffer_TerrainTexture.h"

USING(Engine)

CVIBuffer_TerrainTexture::CVIBuffer_TerrainTexture(
	LPDIRECT3DDEVICE9 pDevice,
	_uint iVertexCountX,
	_uint iVertexCountZ,
	float fVertexInterval/* = 1.f*/)
	: CVIBuffer(pDevice)
	, m_iVertexCountX(iVertexCountX)
	, m_iVertexCountZ(iVertexCountZ)
	, m_fVertexInterval(fVertexInterval)
{
}

HRESULT CVIBuffer_TerrainTexture::ReadyComponentPrototype()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iVertexCount = m_iVertexCountX * m_iVertexCountZ;
	m_iFVF = FVF_VTX_TEXTURE;
	m_iTriCount = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::ReadyComponentPrototype()))
		return E_FAIL;

	VTX_TEXTURE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_uint iIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ; ++z)
	{
		for (_uint x = 0; x < m_iVertexCountX; ++x)
		{
			iIndex = z * m_iVertexCountX + x;

			if (m_iVertexCount <= iIndex)
				continue;

			pVertex[iIndex].vPosition = D3DXVECTOR3(x * m_fVertexInterval, 0.f, z * m_fVertexInterval);
			pVertex[iIndex].vUV = D3DXVECTOR2(x / (m_iVertexCountX - 1.f), 1.f - (z / (m_iVertexCountZ - 1.f)));
		}
	}

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);	

	_uint iTriIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ - 1; ++z)
	{
		for (_uint x = 0; x < m_iVertexCountX - 1; ++x)
		{
			iIndex = z * m_iVertexCountX + x;

			if (m_iVertexCount <= iIndex)
				continue;	

			pIndex[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndex[iTriIndex]._2 = iIndex + m_iVertexCountX + 1;
			pIndex[iTriIndex++]._3 = iIndex + 1;

			pIndex[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndex[iTriIndex]._2 = iIndex + 1;
			pIndex[iTriIndex++]._3 = iIndex;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::ReadyComponent(void * pArg)
{
	if (FAILED(CVIBuffer::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 장치에게 인덱스버퍼 셋 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
		return E_FAIL;

	/* 인덱스를 이용해서 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

_uint * CVIBuffer_TerrainTexture::LoadHeightMap(TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;

	DWORD dwBytes = 0;
	BITMAPFILEHEADER BmpFileHeader;
	ReadFile(hFile, &BmpFileHeader, sizeof(BITMAPFILEHEADER), &dwBytes, 0);

	BITMAPINFOHEADER BmpInfoHeader;
	ReadFile(hFile, &BmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytes, 0);

	_uint iMaxCount = BmpInfoHeader.biWidth * BmpInfoHeader.biHeight; // 이미지 크기
	_uint* pPixels = new _uint[iMaxCount];							  // 이미지마다 크기가 다르니까 동적 배열로
	ReadFile(hFile, pPixels, sizeof(_uint) * iMaxCount, &dwBytes, 0);

	CloseHandle(hFile);

	return pPixels;
}

CVIBuffer_TerrainTexture* CVIBuffer_TerrainTexture::Create(
	LPDIRECT3DDEVICE9 pDevice, 
	_uint iVertexCountX, 
	_uint iVertexCountZ,
	float fVertexInterval/* = 1.f*/)
{
	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(pDevice, iVertexCountX, iVertexCountZ, fVertexInterval);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_TerrainTexture");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_TerrainTexture::Clone(void * pArg)
{
	CVIBuffer_TerrainTexture* pClone = new CVIBuffer_TerrainTexture(*this);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_TerrainTexture");
		SafeRelease(pClone);
	}
	SafeAddRef(m_pDevice);

	return pClone;
}

void CVIBuffer_TerrainTexture::Free()
{
	CVIBuffer::Free();
}
