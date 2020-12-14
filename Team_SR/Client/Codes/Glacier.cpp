#include "stdafx.h"
#include "..\Headers\Glacier.h"


CGlacier::CGlacier(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}


HRESULT CGlacier::ReadyGameObjectPrototype()
{
	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlacier::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	m_wstrTextureKey = L"Component_Texture_GlacierMove";
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 15;

	
	return S_OK;
}

_uint CGlacier::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);

	if (GetAsyncKeyState('M'))
		m_bHit = true;

	if (FAILED(Movement(fDeltaTime)))
		return 0;

	m_pTransformCom->UpdateTransform();

	return _uint();
}

_uint CGlacier::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	m_bFrameLoopCheck = Frame_Move(fDeltaTime);

	return _uint();
}

HRESULT CGlacier::RenderGameObject()
{

	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
		return E_FAIL;

	if (FAILED(Set_Texture()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlacier::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

	CTexture* pTexture;
	// 플라이
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_GlacierMove",
		L"Com_Texture",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(L"Component_Texture_GlacierMove", pTexture);

	return S_OK;
}

HRESULT CGlacier::Set_Texture()
{
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

HRESULT CGlacier::Movement(float fDeltaTime)
{
	const _vector vPlayerPos = m_pPlayer->GetTransform()->m_TransformDesc.vPosition;
	const _vector vGlacierPos = m_pTransformCom->m_TransformDesc.vPosition;
	_vector vLook = vPlayerPos - vGlacierPos;
	float fPlayerYDegree = m_pPlayer->GetTransform()->m_TransformDesc.vRotation.y;
	float fLookLength = D3DXVec3Length(&vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	float fTestSpeed = 10.f;

	if ((fLookLength > 5 && (fLookLength < 60 || true == m_bHit)))
		m_pTransformCom->m_TransformDesc.vPosition += vLook * fDeltaTime * fTestSpeed;

	return S_OK;
}

CGlacier* CGlacier::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CGlacier* pInstance = new CGlacier(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CGlacier");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CGlacier::Clone(void* pArg/* = nullptr*/)
{
	CGlacier* pClone = new CGlacier(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CGlacier");
		SafeRelease(pClone);
	}

	return pClone;
}

void CGlacier::Free()
{
	for (auto& rPair : m_mapTexture)
		SafeRelease(rPair.second);
	m_mapTexture.clear();

	SafeRelease(m_pVIBufferCom);

	CGameObject::Free();
}
