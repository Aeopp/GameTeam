#include "..\Headers\Renderer.h"
#include "GameObject.h"

USING(Engine)


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(m_pDevice);

	// ��ġ�� �̸� ����.
	m_pDevice->GetDeviceCaps(&_Caps9);
}

/* �� �����Ӹ��� ���� ����Ʈ�� ������Ʈ�� �߰��Ѵ�. */
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

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	// ������ ������ ���� ���� �׻� ����ȭ
	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	// ���� ����ŧ�� �ѱ�
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	for (size_t i = 0; i < MaxTexState; ++i)
	{
		// �ؽ�ó ���͸� �̵�漺 (�ִ� ǰ��)
		m_pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, _Caps9.MaxAnisotropy);
		// �Ӹ� ���� ������		 
		m_pDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}
	

	if (FAILED(RenderPriority()))
		return E_FAIL;

	if (FAILED(RenderNoAlpha()))
		return E_FAIL;

	if (FAILED(RenderAlpha()))
		return E_FAIL;

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
	���� �׽��� ==================================================================
	*/
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1); /*���ı��ذ�*/
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
	���� ���� ==================================================================
	*/
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	///*
	//D3DRS_SRCBLEND: ���� �׷����ߵ� �ȼ��� ARGB
	//D3DRS_DESTBLEND: �̹� �׷����ִ� �ȼ�	 ARGB
	//D3DBLEND_SRCALPHA: ȥ�պ��� ���� 0~1 ����. (As, As, As, As)
	//D3DBLEND_INVSRCALPHA: ȥ�պ��� ���� 0~1 ����. (1-As, 1-As, 1-As, 1-As)
	//�����ȼ� = D3DRS_SRCBLEND * D3DBLEND_SRCALPHA + D3DRS_DESTBLEND * D3DBLEND_INVSRCALPHA
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
