#include "..\Headers\VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

HRESULT CVIBuffer::ReadyComponentPrototype()
{
	/* ���ؽ� ���� ���� */
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iVertexSize * m_iVertexCount, /* ���ؽ����۰� ������ �迭�� �� ������ */
		0, /* 0�̸� �������� */
		m_iFVF, /* FVF */
		D3DPOOL_MANAGED, /* �޸� ���� ��� */
		&m_pVB, /* �Ҵ�� ���ؽ������� �ּҸ� ��ȯ */
		nullptr)))
	{
		return E_FAIL;
	}

	/* �ε������� ���� */
	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iIndexSize * m_iTriCount, /* �ε������۰� ������ �迭�� �� ������ */
		0, /* 0�̸� �������� */
		m_IndexFormat, /* �ε��� ���� */
		D3DPOOL_MANAGED, /* �޸� ���� ��� */
		&m_pIB,
		nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::ReadyComponent(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render_VIBuffer()
{
	/* ��ġ���� ���ؽ����� ���� */
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
		return E_FAIL;

	/* ��ġ���� FVF ���� */
	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	/* �ڽ��� ���ҽ� ���� */
	SafeRelease(m_pVB);
	SafeRelease(m_pIB);

	/* �θ��� free */
	CComponent::Free();
}
