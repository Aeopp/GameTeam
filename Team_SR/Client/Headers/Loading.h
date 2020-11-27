#pragma once
#ifndef __LOADING_H__

#include "Base.h"
class CLoading final : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice, ESceneID eNextSceneID);
	virtual ~CLoading() = default;	

public:
	_bool IsFinished() const;

public:
	HRESULT ReadyLoading();

private:
	HRESULT ReadyStageResources();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, ESceneID eNextSceneID);
	virtual void Free() override;

public:
	static unsigned __stdcall ThreadMain(void* pParam);

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	ESceneID m_eNextSceneID;

	HANDLE	m_hThread;
	CRITICAL_SECTION m_CriticalSec;

	_bool	m_IsFinished = false;

};

#define __LOADING_H__
#endif
