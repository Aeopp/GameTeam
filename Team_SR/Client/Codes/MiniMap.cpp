#include "stdafx.h"
#include "..\Headers\MiniMap.h"
#include "DXWrapper.h"
#include "MainCamera.h"
#include "Vertexs.h"


CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{}

HRESULT CMiniMap::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	/*	- 0.49000000953674, - 0.5 ,- 0.54099988937378        ;
		- 1.49000000953674, - 0.5 ,0.45900011062622			 ;
		- 1.49000000953674, - 0.5 ,- 0.54099988937378		 ;
		- 0.49000000953674, - 0.5 ,0.45900011062622			 ;
	
		 0.03, 0.03, 0.03          ;
		 0.97, 0.97, 0.03		   ;
		 0.97, 0.03, 0.03		   ;
		 0.03, 0.97, 0.03		   ;*/

	std::vector<Vertex::Location2DUV> _Vertexs(6);

	_Vertexs[0].Location={ -1.49000000953674, -0.5 ,-0.54099988937378 };
	_Vertexs[0].UV = { 0.97, 0.03 };

	_Vertexs[1].Location = { -1.49000000953674, -0.5 ,0.45900011062622 };
	_Vertexs[1].UV = { 0.97, 0.97 };

	_Vertexs[2].Location = { -0.49000000953674, -0.5 ,-0.54099988937378 };
	_Vertexs[2].UV = { 0.03, 0.03 };

	_Vertexs[3].Location = { -0.49000000953674, -0.5 ,0.45900011062622 };
	_Vertexs[3].UV = { 0.03, 0.97 };

	_Vertexs[4].Location = { -0.49000000953674, -0.5 ,-0.54099988937378 };
	_Vertexs[4].UV = { 0.03, 0.03 };

	_Vertexs[5].Location = { -1.49000000953674, -0.5 ,0.45900011062622 };
	_Vertexs[5].UV = { 0.97, 0.97 };

	CreateVertex(m_pDevice, _Vertexs, PlayerMarkerVertexCount, PlayerMarkerTriangleCount,
		PlayerMarkerVertexByteSize, PlayerMarkerVertexBuf, PlayerMarkerVertexDecl);

	PlayerMarkerTexture = std::shared_ptr<IDirect3DTexture9>(
		LOAD_TEXTURE(m_pDevice, L"..\\Resources\\Player\\MapMarker\\map_marker.png"),
		[](IDirect3DTexture9*const _Target)
		{
			_Target->Release();
		});

	return S_OK;
}

HRESULT CMiniMap::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	auto*const _InitTarget = reinterpret_cast<CMiniMap::InitializeInfo*>(pArg);
	MiniMapScale = { _InitTarget->MapWorld._11 ,  _InitTarget->MapWorld._22, _InitTarget->MapWorld._33 };

	std::vector<Vertex::OnlyLocation> _MiniMapPoints;
	std::transform(
		std::make_move_iterator(std::begin(_InitTarget->_Points)) , 
		std::make_move_iterator( std::end(_InitTarget->_Points) ) , 
		std::back_inserter(_MiniMapPoints) ,[](vec3& ConvertLocation) 
		{
			return Vertex::OnlyLocation{ ConvertLocation };
		} );

	CreateVertex(m_pDevice, _MiniMapPoints, 
		MiniMapVertexCount, MiniMapTriangleCount, 
		MiniMapVertexByteSize, MiniMapVertexBuf, MiniMapVertexDecl);

	return S_OK;
}

_uint CMiniMap::UpdateGameObject(float fDeltaTime)
{
	Super::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CMiniMap::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::UI, this)))
		return 0;

	return _uint();
}

HRESULT CMiniMap::RenderGameObject()
{
	if (FAILED(Super::RenderGameObject()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	mat MiniMapProjection;
	D3DXMatrixOrthoLH(&MiniMapProjection, WINCX, WINCY, 0.0f, 10000.f);
	auto _Camera = dynamic_cast<CMainCamera*>(m_pManagement->GetGameObject(-1, L"Layer_MainCamera", 0));

	// MiniMap ShaderTest
	{
		auto& _MiniMapEffect = Effect::GetEffectFromName(L"MiniMap");
		mat MiniMapWorld;
		MiniMapWorld = MATH::WorldMatrix(MiniMapScale*3.f, { 0,0,0 }, { -200,+200,2.0f });

		_MiniMapEffect.SetVSConstantData(m_pDevice, "World", MiniMapWorld);
		_MiniMapEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);
		m_pDevice->SetVertexShader(_MiniMapEffect.VsShader);
		m_pDevice->SetPixelShader(_MiniMapEffect.PsShader);

		m_pDevice->SetStreamSource(0, MiniMapVertexBuf.get(), 0, MiniMapVertexByteSize);
		m_pDevice->SetVertexDeclaration(MiniMapVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MiniMapTriangleCount);

	};

	{
		auto& _UITextureEffect = Effect::GetEffectFromName(L"UITexture");
		mat PlayerMarkerWorld;
		PlayerMarkerWorld = MATH::WorldMatrix({ 10,10,10}, { 0,0,0 }, { -200,+200,2.0f });

		_UITextureEffect.SetVSConstantData(m_pDevice, "World", PlayerMarkerWorld);
		_UITextureEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);

		m_pDevice->SetTexture(_UITextureEffect.GetTexIdx("DiffuseSampler"), PlayerMarkerTexture.get());
		m_pDevice->SetVertexShader(_UITextureEffect.VsShader);
		m_pDevice->SetPixelShader(_UITextureEffect.PsShader);

		m_pDevice->SetStreamSource(0, PlayerMarkerVertexBuf.get(), 0, PlayerMarkerVertexByteSize);
		m_pDevice->SetVertexDeclaration(PlayerMarkerVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
	}


	m_pDevice->SetVertexShader(nullptr);
	m_pDevice->SetPixelShader(nullptr);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMiniMap* pInstance = new CMiniMap(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMiniMap");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMiniMap::Clone(void* pArg/* = nullptr*/)
{
	CMiniMap* pClone = new CMiniMap(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMiniMap::Free()
{
	Super::Free();
}
