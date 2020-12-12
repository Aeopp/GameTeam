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

	if (nullptr != pArg)
	{
		// 구조체 크기 검사
		if (sizeof(MonsterBasicArgument) == *static_cast<_uint*>(pArg))
		{
			MonsterBasicArgument* pArgument = static_cast<MonsterBasicArgument*>(pArg);
			m_pPlayer = (CGameObject*)(pArgument->pPlayer);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
		}
	}

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

HRESULT CMonster::AddComponents()
{
	/* For.Com_VIBuffer */
	if (FAILED(CGameObject::AddComponent(
		STATIC,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

// 플레이어 인식 - 인식하면 TRUE, 인식하지 못하면 FALSE
BOOL CMonster::PlayerAwareness()
{
	// m_pPlayer->

	return FALSE;
}

void CMonster::Free()
{
	SafeRelease(m_pVIBufferCom);
	// TODO :: 텍스쳐 컴포넌트 추가 이후 주석 풀기
	//SafeRelease(m_pTextureCom);

	CGameObject::Free();
}


