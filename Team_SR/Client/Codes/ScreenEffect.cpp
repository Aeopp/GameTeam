#include "stdafx.h"
#include "..\Headers\ScreenEffect.h"
#include "DXWrapper.h"
#include "Vertexs.h"

CScreenEffect::CScreenEffect(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{}

HRESULT CScreenEffect::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	std::vector<Vertex::Location2DUV> _Vertexs(6);

	_Vertexs[0].Location = { -0.5f, -0.5f,0.0f };
	_Vertexs[0].UV = { 0.03f, 0.97f };

	_Vertexs[1].Location = { -0.5f, 0.5f ,0.0f };
	_Vertexs[1].UV = { 0.03f, 0.03f };

	_Vertexs[2].Location = { +0.5f,0.5f,  0.0f };
	_Vertexs[2].UV = { 0.97f, 0.03f };

	_Vertexs[3].Location = { -0.5f, -0.5f, 0.0f };
	_Vertexs[3].UV = { 0.03f, 0.97f };

	_Vertexs[4].Location = { +0.5f, +0.5f, 0.0f };
	_Vertexs[4].UV = { 0.97f , 0.03f };

	_Vertexs[5].Location = { +0.5f, -0.5f, 0.0f };
	_Vertexs[5].UV = { 0.97f, 0.97f };

	CreateVertex(m_pDevice, _Vertexs,   VertexCount, 
		                                TriangleCount, 
		                                VertexByteSize, 
		                                VertexBuf, 
										VertexDecl);

	Textures[L"Vignette"] = std::shared_ptr<IDirect3DTexture9>
		(
			LOAD_TEXTURE(m_pDevice, L"..\\Resources\\ScreenEffect\\vignette.png"),
			[](IDirect3DTexture9* const _Target)
		{
				if(_Target)
					_Target->Release();
		});
	
		Textures[L"Freeze"] = std::shared_ptr<IDirect3DTexture9>
		(
			LOAD_TEXTURE(m_pDevice, L"..\\Resources\\ScreenEffect\\Freeze.png"),
			[](IDirect3DTexture9* const _Target)
			{
				if (_Target)
					_Target->Release();
			});

	Textures[L"Blood"] = std::shared_ptr<IDirect3DTexture9>
		(
			LOAD_TEXTURE(m_pDevice, L"..\\Resources\\ScreenEffect\\Blood.png"),
			[](IDirect3DTexture9* const _Target)
			{				
				if (_Target)
					_Target->Release();
			});

	Textures[L"Cross"] = std::shared_ptr<IDirect3DTexture9>
		(
			LOAD_TEXTURE(m_pDevice, L"..\\Resources\\ScreenEffect\\Cross.png"),
			[](IDirect3DTexture9* const _Target)
			{
				if (_Target)
					_Target->Release();
			});



	for (size_t i = 0; i < 9; ++i)
	{
		const std::wstring Idx = std::to_wstring(i); 
		const std::wstring TextureKey = L"Shield" + Idx;
		Textures[TextureKey] = std::shared_ptr<IDirect3DTexture9>
			(
				LOAD_TEXTURE(m_pDevice, L"..\\Resources\\ScreenEffect\\Shield\\" + Idx + L".png"),
				[](IDirect3DTexture9* const _Target)
				{
					if (_Target)
						_Target->Release();
				});
	}

	return S_OK;
}

HRESULT CScreenEffect::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CScreenEffect::UpdateGameObject(float fDeltaTime)
{
	IteminteractionEffecT -= fDeltaTime;
	BloodEffectT -= fDeltaTime;
	BlurTime -= fDeltaTime;
	FreezeTime -= fDeltaTime;

	return Super::UpdateGameObject(fDeltaTime);
}

_uint CScreenEffect::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::ScreenPostEffect, this)))
		return 0;

	return _uint();
}

HRESULT CScreenEffect::RenderGameObject()
{
	if (FAILED(Super::RenderGameObject()))
		return E_FAIL;

	mat proj;
	D3DXMatrixOrthoLH(&proj, 
		static_cast<float> (WINCX), 
		static_cast <float> (WINCY),
		0.0f, 
		(std::numeric_limits<float>::max)());
	
        auto& _ScreenPostEffect = Effect::GetEffectFromName(L"ScreenPostEffect");
        mat World;
        World = MATH::WorldMatrix({ 1920,1080,0.0f }, { 0,0,0 }, { 0.0f,0.0f,0.0f });

		_ScreenPostEffect.SetVSConstantData(m_pDevice, "World", World);
		_ScreenPostEffect.SetVSConstantData(m_pDevice, "Projection", proj);
		_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0l);
		m_pDevice->SetVertexShader(_ScreenPostEffect.VsShader);
		m_pDevice->SetPixelShader(_ScreenPostEffect.PsShader);
		m_pDevice->SetStreamSource(0, VertexBuf.get(), 0, VertexByteSize);
		m_pDevice->SetVertexDeclaration(VertexDecl.get());
		
		if (FreezeTime >= 0.0f)
		{
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft", 1.0f);
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 1l);
			m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Freeze"].get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0l);
		}
		else if (BlurTime >=0.0f)
		{
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft", 1.0f);
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0l);

			m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Vignette"].get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		}
		else if(IteminteractionEffecT>=0.0f)
		{
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft", IteminteractionEffecT);
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0l);
			m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Vignette"].get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		}

		if (BloodEffectT>= 0.0f)
		{
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft",  BloodEffectT * BloodEffectT);
			_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0l);
			m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Blood"].get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);
		};

		_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft", 1.f);
		_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0);
		m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Shield" + std::to_wstring(ShieldImgIdx)].get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);



		World = MATH::WorldMatrix({ 50,50,0.0f }, { 0,0,0 }, { 0.0f,0.0f,0.0f });
		_ScreenPostEffect.SetVSConstantData(m_pDevice, "World", World);
		_ScreenPostEffect.SetPSConstantData(m_pDevice, "AlphaCoefft", 1.f);
		_ScreenPostEffect.SetPSConstantData(m_pDevice, "Flag", 0);
		m_pDevice->SetTexture(_ScreenPostEffect.GetTexIdx("DiffuseSampler"), Textures[L"Cross"].get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);

	return S_OK;
}

CScreenEffect* CScreenEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CScreenEffect* pInstance = new CScreenEffect(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create ScreenEffect");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CScreenEffect::Clone(void* pArg/* = nullptr*/)
{
	CScreenEffect* pClone = new CScreenEffect(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CScreenEffect::Free()
{
	Super::Free();
}

void CScreenEffect::ItemInteractionEffect()
{
	IteminteractionEffecT = 2.0f;
}

void CScreenEffect::BloodEffect()
{
	BloodEffectT = 1.0f;
}

void CScreenEffect::FreezeEffect()
{
	FreezeTime = 0.1f;
}

void CScreenEffect::Blur()&
{
	BlurTime = 0.1f;
}

void CScreenEffect::Shield(const uint8_t ImgIdx)&
{
	ShieldImgIdx = ImgIdx;
}
