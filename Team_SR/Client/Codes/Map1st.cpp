#include "stdafx.h"
#include "Map1st.h"
#include "DXWrapper.h"


CMap1st::CMap1st(LPDIRECT3DDEVICE9 pDevice)
	:Super(pDevice)
{
};

HRESULT CMap1st::ReadyGameObjectPrototype()
{
	if (FAILED(Super::ReadyGameObjectPrototype()))
		return E_FAIL;

	mat MapWorld  = MATH::WorldMatrix({ 7,7,7 }, { 0,0,0}, { 0,0,0});

	LoadMap(L"..\\Resources\\Map\\1\\", MapWorld);
	_ShaderInfo = Shader::CompileAndCreate(m_pDevice, L"..\\Shader\\NormalMapping");

	_ShaderInfo.VsHandleMap = Shader::ConstantHandleInitialize(
		_ShaderInfo.VsTable,
		std::vector<std::string>{
		"WorldMatrix",
		"WorldViewProjectionMatrix",
		"WorldLightLocation",
		"WorldCameraLocation",
		});

	_ShaderInfo.PsHandleMap= Shader::ConstantHandleInitialize(
		_ShaderInfo.PsTable,
		std::vector<std::string>{
		"LightColor"
	});

	_ShaderInfo.TextureDescMap = Shader::ConstantHandleDescInitialize(_ShaderInfo.PsTable,
		{ "DiffuseSampler",
		"SpecularSampler",
		"NormalSampler" });

	MapAmbient = 0x00202020;

	return S_OK;
}

HRESULT CMap1st::ReadyGameObject(void * pArg)
{
	if (FAILED(Super::ReadyGameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->m_TransformDesc.vScale={ 1,1,1 };

	return S_OK;
}

_uint CMap1st::UpdateGameObject(float fDeltaTime)
{
	Super::UpdateGameObject(fDeltaTime);

	
	return _uint();
}

_uint CMap1st::LateUpdateGameObject(float fDeltaTime)
{
	Super::LateUpdateGameObject(fDeltaTime);

	return _uint();
}

HRESULT CMap1st::RenderGameObject()
{
	Super::RenderGameObject();

	return S_OK;
}

CMap1st* CMap1st::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMap1st* pInstance = new CMap1st(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CMapBase");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CMap1st::Clone(void * pArg)
{
	CMap1st* pClone = new CMap1st(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	_ShaderInfo.AddRef();
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CTerrain");
		SafeRelease(pClone);
	}

	return pClone;
}

void CMap1st::Free()
{
	_ShaderInfo.Release();
	Super::Free();
}
