#include "..\Headers\Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice, TCHAR* pFilePath, _uint iCount/* = 1*/)
	: CComponent(pDevice)
	, m_pFilePath(pFilePath)
	, m_iCount(iCount)
{
}

HRESULT CTexture::ReadyComponentPrototype()
{		
	if (FAILED(CComponent::ReadyComponentPrototype()))
		return E_FAIL;

	TCHAR szRealPath[128] = L"";
	IDirect3DBaseTexture9* pTexture = nullptr;
	HRESULT hr = 0;

	m_Textures.reserve(m_iCount);

	for (_uint i = 0; i < m_iCount; ++i)
	{
		//m_pFilePath = L"../Texture/태후니%d.png";
		swprintf_s(szRealPath, m_pFilePath, i);
		hr = D3DXCreateTextureFromFile(
		m_pDevice,
		szRealPath,
		(LPDIRECT3DTEXTURE9*)&pTexture);

		if (FAILED(hr))
		{
			TCHAR szLogMessage[256] = L"";
			swprintf_s(szLogMessage, L"Failed To Load Texture %s", szRealPath);
			return E_FAIL;
		}

		m_Textures.push_back(pTexture);		
	}

	return S_OK;
}

HRESULT CTexture::ReadyComponent(void* pArg/* = nullptr*/)
{
	if (FAILED(CComponent::ReadyComponent(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTexture::Set_Texture(_uint iIndex)
{
	if (m_Textures.size() <= iIndex)
		return E_FAIL;

	/* 
	버텍스 그리기 이전에 그리려는 버텍스에 
	맵핑시킬 텍스처를 장치에게 전달 
	*/
	if (FAILED(m_pDevice->SetTexture(0, m_Textures[iIndex])))
		return E_FAIL;

	return S_OK;
}

CTexture * CTexture::Create(
	LPDIRECT3DDEVICE9 pDevice, 
	TCHAR * pFilePath, 
	_uint iCount)
{
	CTexture* pInstance = new CTexture(pDevice, pFilePath, iCount);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Texture");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg/* = nullptr*/)
{
	CTexture* pInstance = new CTexture(*this);
	SafeAddRef(m_pDevice);
	for (auto& pTexture : m_Textures)
	{
		SafeAddRef(pTexture);
	}
	if (FAILED(pInstance->ReadyComponent(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone Texture");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
	{
		SafeRelease(pTexture);
	}

	m_Textures.clear();

	CComponent::Free();
}
