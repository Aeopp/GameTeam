#include "..\Headers\Renderer.h"
#include "GameObject.h"

USING(Engine)


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(m_pDevice);
}

/* 매 프레임마다 렌더 리스트에 오브젝트를 추가한다. */
HRESULT CRenderer::AddGameObjectInRenderer(ERenderID eID, CGameObject * pGameObject)
{
	if (0 > (_int)eID ||
		ERenderID::MaxCount <= eID)
	{
		PRINT_LOG(L"Error", L"Out of range Render list");
		return E_FAIL;
	}

	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[(_int)eID].push_back(pGameObject);
	SafeAddRef(pGameObject);

	return E_NOTIMPL;
}

HRESULT CRenderer::Render(HWND hWnd)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	if (FAILED(RenderPriority()))
		return E_FAIL;

	if (FAILED(RenderNoAlpha()))
		return E_FAIL;

	if (FAILED(RenderAlpha()))
		return E_FAIL;

	if (FAILED(RenderUI()))
		return E_FAIL;

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

	return S_OK;
}

HRESULT CRenderer::RenderPriority()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::Priority])
	{
		if (FAILED(pObject->RenderGameObject()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::Priority].clear();

	return S_OK;
}

HRESULT CRenderer::RenderNoAlpha()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::NoAlpha])
	{
		if (FAILED(pObject->RenderGameObject()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::NoAlpha].clear();

	return S_OK;
}

HRESULT CRenderer::RenderAlpha()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::Alpha])
	{
		if (FAILED(pObject->RenderGameObject()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::Alpha].clear();

	return S_OK;
}

HRESULT CRenderer::RenderUI()
{
	for (auto& pObject : m_GameObjects[(_int)ERenderID::UI])
	{
		if (FAILED(pObject->RenderGameObject()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::UI].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	return new CRenderer(pDevice);
}

void CRenderer::Free()
{
	for (_int i = 0; i < (_int)ERenderID::MaxCount; ++i)
	{
		for (auto& pObject : m_GameObjects[i])
		{
			SafeRelease(pObject);
		}

		m_GameObjects[i].clear();
	}

	SafeRelease(m_pDevice);
}
