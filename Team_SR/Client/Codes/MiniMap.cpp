#include "stdafx.h"
#include "..\Headers\MiniMap.h"
#include "DXWrapper.h"
#include "MainCamera.h"
#include "Vertexs.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"

const vec2 CMiniMap::MiniMapModeFirstScreenOffset
{
	-750.0f ,+250.0f 
};
const vec2 CMiniMap::MiniMapModeSecondScreenOffset
{
	-750.0f ,-200.0f
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
		if(_Target)
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
		std::make_move_iterator(std::end(_InitTarget->_Points) ) , 
		std::back_inserter(_MiniMapPoints) ,[](vec3& ConvertLocation) 
		{
			return Vertex::OnlyLocation{ ConvertLocation };
		});

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

	for (auto& _CurrentElement : m_pManagement->GetGameObjects(-1, L"Layer_"+TYPE_NAME<CMonster>()))
	{
		auto* const _CurrentMonster = dynamic_cast<CMonster* const>(_CurrentElement);

		if (!_CurrentMonster->IsDead())
		{
			_MonsterLocations.push_back(MiniRenderInfo{ _CurrentMonster->GetTransform()->GetLocation(), _CurrentMonster->RotationXZPlane, vec4{ 1.0f,0.3f,0.34f,1.f } });
		}
	}

	for (auto& _CurrentElement : m_pManagement->GetGameObjects(-1, L"Layer_" + TYPE_NAME<CItem>()))
	{
		auto* const _CurrentItem= dynamic_cast<CItem* const>(_CurrentElement);
		
		const vec4 _CurItemColorCoefft = { MATH::RandReal({0.1f,1.f}),MATH::RandReal({0.1f,1.f}),MATH::RandReal({0.1f,1.f}),1.0f };
		if (!_CurrentItem->IsAcheive())
		{
			_ItemLocations.push_back(MiniRenderInfo{ _CurrentItem->GetTransform()->GetLocation() ,  0.0f, _CurItemColorCoefft });
		}
	}

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

	_MonsterLocations.clear();
	_MonsterLocations.shrink_to_fit();
	_ItemLocations.clear();
	_ItemLocations.shrink_to_fit();

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
	CMiniMap* pClone = new CMiniMap(*this); 
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

	{
		auto& _MiniMapEffect = Effect::GetEffectFromName(L"MiniMap");
		mat MiniMapWorld;
		MiniMapWorld = MATH::WorldMatrix(MiniMapScale* MiniMapScaleCorrection, { 0,0,0 }, { MiniMapModeFirstScreenOffset.x,MiniMapModeFirstScreenOffset.y,0.0f });

		_MiniMapEffect.SetVSConstantData(m_pDevice, "World", MiniMapWorld);
		_MiniMapEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);
		_MiniMapEffect.SetPSConstantData(m_pDevice, "PlayerScreenLocation", vec2{ 0,0 });
		_MiniMapEffect.SetPSConstantData(m_pDevice, "DistanceMin", 1000.0f);
		_MiniMapEffect.SetPSConstantData(m_pDevice, "Relative", 0l);
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
		_UITextureEffect.SetPSConstantData(m_pDevice, "RenderFlag", 0l);
		_UITextureEffect.SetPSConstantData(m_pDevice, "ColorCoefft", vec4{1.f,1.f ,1.f ,1.f });

		m_pDevice->SetTexture(_UITextureEffect.GetTexIdx("DiffuseSampler"), PlayerMarkerTexture.get());
		m_pDevice->SetVertexShader(_UITextureEffect.VsShader);
		m_pDevice->SetPixelShader(_UITextureEffect.PsShader);

		m_pDevice->SetStreamSource(0, PlayerMarkerVertexBuf.get(), 0, PlayerMarkerVertexByteSize);
		m_pDevice->SetVertexDeclaration(PlayerMarkerVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);

		RenderInformationsFirst(1l,MiniMapScaleCorrection , vec2{ MiniMapModeFirstScreenOffset.x , MiniMapModeFirstScreenOffset.y }, vec3{ 7,7,7}, _ItemLocations, _UITextureEffect);
		RenderInformationsFirst(0l,MiniMapScaleCorrection , vec2{ MiniMapModeFirstScreenOffset.x , MiniMapModeFirstScreenOffset.y }, vec3{ 10,10,10}, _MonsterLocations, _UITextureEffect);
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
		MiniMapWorld = MATH::WorldMatrix(MiniMapScale * MiniMapScaleCorrection, { 0,0,0 }, { -PlayerLocation.x * MiniMapScaleCorrection,-PlayerLocation.z * MiniMapScaleCorrection,0.0f });
		MiniMapWorld *= MATH::RotationMatrixFromAxis({ 0,0,1 }, Yaw);
		MiniMapWorld._41 += MiniMapModeSecondScreenOffset.x;
		MiniMapWorld._42 += MiniMapModeSecondScreenOffset.y;

		_MiniMapEffect.SetVSConstantData(m_pDevice, "World", MiniMapWorld);
		_MiniMapEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);
		_MiniMapEffect.SetPSConstantData(m_pDevice, "Relative", 1l);
		_MiniMapEffect.SetPSConstantData(m_pDevice, "PlayerScreenLocation", vec2{ MiniMapModeSecondScreenOffset.x ,MiniMapModeSecondScreenOffset.y });
		_MiniMapEffect.SetPSConstantData(m_pDevice, "DistanceMin", 200.0f);

		m_pDevice->SetVertexShader(_MiniMapEffect.VsShader);
		m_pDevice->SetPixelShader(_MiniMapEffect.PsShader);

		m_pDevice->SetStreamSource(0, MiniMapVertexBuf.get(), 0, MiniMapVertexByteSize);
		m_pDevice->SetVertexDeclaration(MiniMapVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MiniMapTriangleCount);
	};

	{
		auto& _UITextureEffect = Effect::GetEffectFromName(L"UITexture");
		mat PlayerMarkerWorld;
		PlayerMarkerWorld = MATH::WorldMatrix({ 20,20,20 }, { 0,0,0 }, { MiniMapModeSecondScreenOffset.x ,MiniMapModeSecondScreenOffset.y,0.0f });

		_UITextureEffect.SetVSConstantData(m_pDevice, "World", PlayerMarkerWorld);
		_UITextureEffect.SetVSConstantData(m_pDevice, "Projection", MiniMapProjection);
		_UITextureEffect.SetPSConstantData(m_pDevice, "RenderFlag", 0l);
		_UITextureEffect.SetPSConstantData(m_pDevice, "ColorCoefft", vec4{ 1.f,1.f ,1.f ,1.f });

		m_pDevice->SetTexture(_UITextureEffect.GetTexIdx("DiffuseSampler"), PlayerMarkerTexture.get());
		m_pDevice->SetVertexShader(_UITextureEffect.VsShader);
		m_pDevice->SetPixelShader(_UITextureEffect.PsShader);
		
		m_pDevice->SetStreamSource(0, PlayerMarkerVertexBuf.get(), 0, PlayerMarkerVertexByteSize);
		m_pDevice->SetVertexDeclaration(PlayerMarkerVertexDecl.get());
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
		RenderInformationsSecond(-Yaw,PlayerLocation,1l, MiniMapScaleCorrection, vec2{ MiniMapModeSecondScreenOffset.x , MiniMapModeSecondScreenOffset.y }, vec3{10,10,10} , _ItemLocations, _UITextureEffect);
		RenderInformationsSecond(-Yaw,PlayerLocation,0l, MiniMapScaleCorrection, vec2{ MiniMapModeSecondScreenOffset.x , MiniMapModeSecondScreenOffset.y }, vec3{ 13,13,13}, _MonsterLocations, _UITextureEffect);
	}
};

void CMiniMap::RenderInformationsFirst(const int32_t RenderFlag,
	const float MiniMapScaleCorrection ,
	const vec2 OffSetXY, 
	const vec3 RenderScale, 
	std::vector<MiniRenderInfo>& _TargetLocations,  
	typename Effect::Info& _CurrentRenderEffect)&
{
	for (const auto& _RenderTargetInfo : _TargetLocations)
	{
		const vec3 _RenderLocation  = _RenderTargetInfo.Location* MiniMapScaleCorrection;
		const float Rotation = _RenderTargetInfo.Rotation;
		mat RenderWorld;
		RenderWorld = MATH::WorldMatrix({ RenderScale.x,RenderScale.y,RenderScale.z}, { 0,0,-Rotation }, { OffSetXY.x + _RenderLocation.x,
																								           OffSetXY.y + _RenderLocation.z,0.0f });
		_CurrentRenderEffect.SetVSConstantData(m_pDevice, "World", RenderWorld);
		_CurrentRenderEffect.SetPSConstantData(m_pDevice, "RenderFlag", RenderFlag);
		_CurrentRenderEffect.SetPSConstantData(m_pDevice, "ColorCoefft", _RenderTargetInfo.ColorCoefft);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
	}
}

void CMiniMap::RenderInformationsSecond(
	const float WorldRelativeRotation,
	const vec3 WorldRelativeLocation,
	const int32_t RenderFlag, 
	const float MiniMapScaleCorrection, 
	const vec2 OffSetXY, 
	const vec3 RenderScale, 
	std::vector<MiniRenderInfo>& _TargetLocations,
	typename Effect::Info& _CurrentRenderEffect)&
{
	for (const auto& _RenderTargetInfo : _TargetLocations)
	{
		const vec3 _RenderLocation = MATH::RotationVec(( _RenderTargetInfo.Location- WorldRelativeLocation) , { 0,1,0 }, WorldRelativeRotation) * MiniMapScaleCorrection;
		const float Rotation = _RenderTargetInfo.Rotation;
		
		const vec2 Lhs = { _RenderLocation.x, _RenderLocation.z };
		const vec2 Rhs = { WorldRelativeLocation.x, WorldRelativeLocation.z };
		const float Distance = D3DXVec2Length(& ( Lhs - Rhs ) );
		mat RenderWorld;

		if (Distance < 200.0f)
		{
			RenderWorld = MATH::WorldMatrix({ RenderScale.x,RenderScale.y,RenderScale.z }, { 0,0,-Rotation - WorldRelativeRotation }, {
																									OffSetXY.x + _RenderLocation.x,
																								   OffSetXY.y + _RenderLocation.z,0.0f });
		}

		_CurrentRenderEffect.SetVSConstantData(m_pDevice, "World", RenderWorld);
		_CurrentRenderEffect.SetPSConstantData(m_pDevice, "RenderFlag", RenderFlag);
		_CurrentRenderEffect.SetPSConstantData(m_pDevice, "ColorCoefft", _RenderTargetInfo.ColorCoefft);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PlayerMarkerTriangleCount);
	}
}
