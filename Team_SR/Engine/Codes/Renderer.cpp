#include "..\Headers\Renderer.h"
#include "GameObject.h"
#include "CollisionComponent.h"

USING(Engine)


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(m_pDevice);

	// 장치를 미리 조사.
	m_pDevice->GetDeviceCaps(&_Caps9);
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

	return S_OK;
}

HRESULT CRenderer::Render(HWND hWnd)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, Ambient);

	for (size_t i = 0; i < MaxTexState; ++i)
	{
		m_pDevice->SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, _Caps9.MaxAnisotropy);
		m_pDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	};

	if (FAILED(RenderPriority()))
		return E_FAIL;

	if (FAILED(RenderNoAlpha()))
		return E_FAIL;

	if (FAILED(RenderAlpha()))
		return E_FAIL;

	CCollisionComponent::CollisionDebugRender(m_pDevice);

	if (FAILED(RenderUI()))
		return E_FAIL;

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
	/*
	알파 테스팅 ==================================================================
	*/
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); /*알파기준값*/
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObject : m_GameObjects[(_int)ERenderID::Alpha])
	{
		if (FAILED(pObject->RenderGameObject()))
			return E_FAIL;

		SafeRelease(pObject);
	}

	m_GameObjects[(_int)ERenderID::Alpha].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	/*
	알파 블렌딩 ==================================================================
	*/
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	///*
	//D3DRS_SRCBLEND: 이제 그려져야될 픽셀의 ARGB
	//D3DRS_DESTBLEND: 이미 그려져있는 픽셀	 ARGB
	//D3DBLEND_SRCALPHA: 혼합비율 값은 0~1 범위. (As, As, As, As)
	//D3DBLEND_INVSRCALPHA: 혼합비율 값은 0~1 범위. (1-As, 1-As, 1-As, 1-As)
	//최종픽셀 = D3DRS_SRCBLEND * D3DBLEND_SRCALPHA + D3DRS_DESTBLEND * D3DBLEND_INVSRCALPHA
	//*/
	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//for (auto& pObject : m_GameObjects[(_int)ERenderID::Alpha])
	//{
	//	if (FAILED(pObject->RenderGameObject()))
	//		return E_FAIL;

	//	SafeRelease(pObject);
	//}

	//m_GameObjects[(_int)ERenderID::Alpha].clear();

	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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

void CRenderer::RegistLight(const D3DLIGHT9& Light)
{

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
