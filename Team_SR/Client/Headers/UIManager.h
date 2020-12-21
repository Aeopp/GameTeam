#pragma once
#ifndef __UIMANAGER_H__

#include "Base.h"

USING(Engine)
class CUIManager final : public CBase
{
	DECLARE_SINGLETON(CUIManager)
private:
	CUIManager();
	virtual ~CUIManager() = default;
public:
	// UI 초기화
	HRESULT ReadyUI();

public:
	// 모든 UI
	void OnAllUI();
	void OffAllUI();

public:
	// 플레이어 정보 - 체력바, 마나, 초상화
	void OnPlayerInfo();
	void OffPlayerInfo();

public:
	// 무기 탄약 - 무기 탄약
	void OnWeaponAmmom();
	void OffWeaponAmmom();

public:
	virtual void Free() override;

private:
	class CPlyerInfoUI* m_pPlayerInfoUI;
	class CWeaponAmmoInfoUI* m_pWeaponAmmoInfoUI;
};

#define __UIMANAGER_H__
#endif