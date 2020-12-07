#include "stdafx.h"
#include "..\Headers\Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

HRESULT CMonster::ReadyGameObjectPrototype()
{
	return S_OK;
}

HRESULT CMonster::ReadyGameObject(void * pArg /*= nullptr*/)
{
	return S_OK;
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
	return S_OK;
}

void CMonster::Free()
{
}
