#include "stdafx.h"
#include "..\Headers\Fire.h"

CFire::CFire(LPDIRECT3DDEVICE9 pDevice)
	:CMonster(pDevice)
{
}

HRESULT CFire::ReadyGameObjectPrototype()
{

	if (FAILED(CMonster::ReadyGameObjectPrototype()))
		return E_FAIL;

	m_wstrTextureKey = CComponent::Tag + TYPE_NAME<CTexture>() + TYPE_NAME<CFire>();

	return S_OK;
}

HRESULT CFire::ReadyGameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CMonster::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->m_TransformDesc.vPosition = *(_vector*)pArg;
		m_pTransformCom->m_TransformDesc.vPosition.y += 1.f;
	}

	m_pTransformCom->m_TransformDesc.vScale = { 1.5f,1.5f,1.5f };

	m_fFrameCnt = 0.f;
	m_fFrameSpeed = 7.f;
	m_fStartFrame = 0.f;
	m_fEndFrame = 21.f;

	return S_OK;
}

_uint CFire::UpdateGameObject(float fDeltaTime)
{
	CMonster::UpdateGameObject(fDeltaTime);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIRE);
	CSoundMgr::Get_Instance()->PlaySound(L"fire_burn.wav", CSoundMgr::FIRE);
	return _uint();
}

_uint CFire::LateUpdateGameObject(float fDeltaTime)
{
	CMonster::LateUpdateGameObject(fDeltaTime);


	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	if (FrameMove_Fire(fDeltaTime)) {
		// 2020.12.17 9:47
		m_byObjFlag ^= static_cast<BYTE>(ObjFlag::Remove);
	}

	return _uint();
}

HRESULT CFire::RenderGameObject()
{
	if (FAILED(CMonster::RenderGameObject()))
		return E_FAIL;

	//if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_TransformDesc.matWorld)))
	//	return E_FAIL;

	//if (FAILED(Set_Texture()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CFire::AddComponents()
{
	if (FAILED(CMonster::AddComponents()))
		return E_FAIL;

	CTexture* pTexture = nullptr;

	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		m_wstrTextureKey,
		L"Com_Texture_Fire",
		(CComponent**)&pTexture)))
		return E_FAIL;
	m_mapTexture.emplace(m_wstrTextureKey, pTexture);

	return S_OK;
}

HRESULT CFire::Set_Texture()
{
	auto iter_find = m_mapTexture.find(m_wstrTextureKey);
	if (m_mapTexture.end() == iter_find)
		return E_FAIL;

	CTexture* pTexture = (CTexture*)iter_find->second;
	// 해당 프레임 텍스처 장치에 셋
	pTexture->Set_Texture((_uint)m_fFrameCnt);

	return S_OK;
}

bool CFire::FrameMove_Fire(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fEndFrame;
		return true;
	}
	return false;
}

CFire * CFire::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CFire* pInstance = new CFire(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CFire");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CFire::Clone(void * pArg /*= nullptr*/)
{
	CFire* pClone = new CFire(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CFire");
		SafeRelease(pClone);
	}

	return pClone;
}

void CFire::Free()
{
	CMonster::Free();
}
