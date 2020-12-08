#include "stdafx.h"
#include "..\Headers\Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

HRESULT CMonster::ReadyGameObjectPrototype()
{
	if (FAILED(CGameObject::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::ReadyGameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMonster::UpdateGameObject(float fDeltaTime)
{
	CGameObject::UpdateGameObject(fDeltaTime);

	return _uint();
}

_uint CMonster::LateUpdateGameObject(float fDeltaTime)
{
	CGameObject::LateUpdateGameObject(fDeltaTime);

	return _uint();
}

HRESULT CMonster::RenderGameObject()
{
	if (FAILED(CGameObject::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

long CMonster::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// TODO :: Static 텍스쳐 컴포넌트 추가하는 로직 으로 변경하기.

	/*if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;*/

		//////////////////////////////////////////////////////
	return S_OK;
}

void CMonster::Free()
{
	SafeRelease(m_pVIBufferCom);
	// TODO :: 텍스쳐 컴포넌트 추가 이후 주석 풀기
	//SafeRelease(m_pTextureCom);

	CGameObject::Free();
}


