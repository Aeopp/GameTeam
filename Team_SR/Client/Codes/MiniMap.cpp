#include "stdafx.h"
#include "..\Headers\MiniMap.h"
#include "DXWrapper.h"
#include "MainCamera.h"
#include "Vertexs.h"
#include "Player.h"
const vec2 CMiniMap::MiniMapModeFirstScreenOffset
{
	-750.0f ,+250.0f 
};
const vec2 CMiniMap::MiniMapModeSecondScreenOffset
{
	-825.0f ,-0.0f
};
CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{}

HRESULT CMiniMap::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;


	std::vector<Vertex::Location2DUV> _Vertexs(6);

	_Vertexs[0].Location = { -0.5f, -0.5f,0.0f};
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

	// 1.
	{
		MiniMapRenderModeFirst(MiniMapProjection);
	}
	// 2.
	{
		MiniMapRenderModeSecond(MiniMapProjection);
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

void CMiniMap::MiniMapRenderModeFirst(mat MiniMapProjection)
{
	static constexpr float  MiniMapScaleCorrection = 2.5f;
	CPlayer* const _CurrentPlayer = dynamic_cast<CPlayer*>(m_pManagement->GetGameObject(-1, L"Layer_Player", 0));
	vec3 PlayerLocation = _CurrentPlayer->GetTransform()->GetLocation();
	PlayerLocation *= MiniMapScaleCorrection;

	float Yaw = _CurrentPlayer->GetTransform()->GetRotation().y;

	// MiniMap ShaderTest
	{
		auto& _MiniMapEffect = Effect::GetEffectFromName(L"MiniMap");
		mat MiniMapWorld;
		MiniMapWorld = MATH::WorldMatrix(MiniMapScale* MiniMapScaleCorrection, { 0,0,0 }, { MiniMapModeFirstScreenOffset.x,MiniMapModeFirstScreenOffset.y,0.0f });

		_MiniMapEffect.SetVSConstantData(m_pDevice, "World", MiniMapWorld);
		_MiniMapEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);
		_MiniMapEffect.SetPSConstantData(m_pDevice, "PlayerScreenLocation", vec2{ 0,0 });
		_MiniMapEffect.SetPSConstantData(m_pDevice, "DistanceMin", 1000.0f);
		m_pDevice->SetVertexShader(_MiniMapEffect.VsShader);
		m_pDevice->SetPixelShader(_MiniMapEffect.PsShader);

		m_pDevice->SetStreamSource(0, MiniMapVertexBuf.get(), 0, MiniMapVertexByteSize);
		m_pDevice->SetVertexDeclaration(MiniMapVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MiniMapTriangleCount);
	};

	
	{
		auto& _UITextureEffect = Effect::GetEffectFromName(L"UITexture");
		mat PlayerMarkerWorld;
		PlayerMarkerWorld = MATH::WorldMatrix({ 15,15,15 }, { 0,0,-Yaw }, { PlayerLocation.x + MiniMapModeFirstScreenOffset.x ,PlayerLocation.z + MiniMapModeFirstScreenOffset.y,0.0f });

		_UITextureEffect.SetVSConstantData(m_pDevice, "World", PlayerMarkerWorld);
		_UITextureEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);

		m_pDevice->SetTexture(_UITextureEffect.GetTexIdx("DiffuseSampler"), PlayerMarkerTexture.get());
		m_pDevice->SetVertexShader(_UITextureEffect.VsShader);
		m_pDevice->SetPixelShader(_UITextureEffect.PsShader);

		m_pDevice->SetStreamSource(0, PlayerMarkerVertexBuf.get(), 0, PlayerMarkerVertexByteSize);
		m_pDevice->SetVertexDeclaration(PlayerMarkerVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
	}
}

void CMiniMap::MiniMapRenderModeSecond(mat MiniMapProjection)
{
	CPlayer* const _CurrentPlayer = dynamic_cast<CPlayer*>(m_pManagement->GetGameObject(-1, L"Layer_Player", 0));
	vec3 PlayerLocation = _CurrentPlayer->GetTransform()->GetLocation();
	float Yaw = _CurrentPlayer->GetTransform()->GetRotation().y;
	static constexpr float MiniMapScaleCorrection = 5.0f;

	   {
			auto& _MiniMapEffect = Effect::GetEffectFromName(L"MiniMap");
			mat MiniMapWorld;
			MiniMapWorld = MATH::WorldMatrix(MiniMapScale * MiniMapScaleCorrection, { 0,0,0 }, { -PlayerLocation.x * MiniMapScaleCorrection,-PlayerLocation.z*MiniMapScaleCorrection,0.0f });
			MiniMapWorld *= MATH::RotationMatrixFromAxis({ 0,0,1 }, Yaw);
			MiniMapWorld._41 += MiniMapModeSecondScreenOffset.x;
			MiniMapWorld._42 += MiniMapModeSecondScreenOffset.y;
				
			_MiniMapEffect.SetVSConstantData(m_pDevice, "World", MiniMapWorld);
			_MiniMapEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);

			/* vec4 BindProjectionPlayerLocation = vec4 {  MiniMapScaleCorrection  ,
																 MiniMapScaleCorrection  ,     0.0f,      1.0f};    
			 D3DXVec4Transform(&BindProjectionPlayerLocation, &BindProjectionPlayerLocation, &MiniMapProjection);

			 BindProjectionPlayerLocation.x /= BindProjectionPlayerLocation.w;
			 BindProjectionPlayerLocation.y /= BindProjectionPlayerLocation.w;
			 BindProjectionPlayerLocation.z /= BindProjectionPlayerLocation.w;*/

			_MiniMapEffect.SetPSConstantData(m_pDevice, "PlayerScreenLocation", vec2{ MiniMapModeSecondScreenOffset.x ,MiniMapModeSecondScreenOffset.y });
			_MiniMapEffect.SetPSConstantData(m_pDevice, "DistanceMin", 110.0f);

			m_pDevice->SetVertexShader(_MiniMapEffect.VsShader);
			m_pDevice->SetPixelShader(_MiniMapEffect.PsShader);

			m_pDevice->SetStreamSource(0, MiniMapVertexBuf.get(), 0, MiniMapVertexByteSize);
			m_pDevice->SetVertexDeclaration(MiniMapVertexDecl.get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MiniMapTriangleCount);

		};

		{
			auto& _UITextureEffect = Effect::GetEffectFromName(L"UITexture");
			mat PlayerMarkerWorld;
			PlayerMarkerWorld = MATH::WorldMatrix({ 20,20,20}, { 0,0,0 }, {  MiniMapModeSecondScreenOffset.x ,MiniMapModeSecondScreenOffset.y,0.0f });

			_UITextureEffect.SetVSConstantData(m_pDevice, "World", PlayerMarkerWorld);
			_UITextureEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);

			m_pDevice->SetTexture(_UITextureEffect.GetTexIdx("DiffuseSampler"), PlayerMarkerTexture.get());
			m_pDevice->SetVertexShader(_UITextureEffect.VsShader);
			m_pDevice->SetPixelShader(_UITextureEffect.PsShader);

			m_pDevice->SetStreamSource(0, PlayerMarkerVertexBuf.get(), 0, PlayerMarkerVertexByteSize);
			m_pDevice->SetVertexDeclaration(PlayerMarkerVertexDecl.get());
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
		}

}
