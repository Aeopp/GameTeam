#include "stdafx.h"
#include "..\Headers\Player.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

HRESULT CPlayer::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::ReadyGameObject(void * pArg)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponent()))
		return E_FAIL;

	return S_OK;
}

_uint CPlayer::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);	

	return _uint();
}

_uint CPlayer::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->AddGameObjectInRenderer(ERenderID::NoAlpha, this)))
		return 0;

	return _uint();
}

HRESULT CPlayer::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::AddComponent()
{

	return S_OK;
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create Player");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void * pArg)
{
	CPlayer* pClone = new CPlayer(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone Player");
		SafeRelease(pClone);
	}

	return pClone;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
