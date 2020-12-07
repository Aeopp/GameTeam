#include "stdafx.h"
#include "..\Headers\Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

HRESULT CMonster::ReadyGameObjectPrototype()
{
	return E_NOTIMPL;
}

HRESULT CMonster::ReadyGameObject(void * pArg /*= nullptr*/)
{
	return E_NOTIMPL;
}

_uint CMonster::UpdateGameObject(float fDeltaTime)
{
	return _uint();
}

_uint CMonster::LateUpdateGameObject(float fDeltaTime)
{
	return _uint();
}

HRESULT CMonster::RenderGameObject()
{
	return E_NOTIMPL;
}

HRESULT CMonster::AddComponents()
{
	return E_NOTIMPL;
}

CGameObject * CMonster::Clone(void * pArg /*= nullptr*/)
{
	return nullptr;
}

void CMonster::Free()
{
}
