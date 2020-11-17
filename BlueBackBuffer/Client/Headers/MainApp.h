#pragma once

#ifndef __MAINAPP_H__

#include "Base.h"

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT ReadyMainApp();

public:
	int UpdateMainApp();
	void Render();
public:
	static CMainApp* Create();
	virtual void Free() override;
};

#define __MAINAPP_H__
#endif