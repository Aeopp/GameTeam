#pragma once

#ifndef __MAINAPP_H__

#include "Base.h"

USING(Engine)
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT ReadyMainApp();

public:
	int UpdateMainApp();

private:
	HRESULT ReadyStaticResources();
	HRESULT ReadyDefaultSetting();

public:
	static CMainApp* Create();
	virtual void Free() override;

private:
	CManagement* m_pManagement = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

#define __MAINAPP_H__
#endif