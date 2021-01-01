#pragma once
#ifndef __UIMANAGER_H__

#include "Base.h"

namespace UI_AddTag
{
	typedef struct tagAddComUIBar
	{
		UI_DESC tUIDesc;
		wstring wsPrototypeTag;
		wstring wsComponentTag;
		//	BOOL bReverse;
		bool bTextOut;
	}UI_BAR_ADD_COMPONENT;

	typedef struct tagAddComUIButton
	{
		UI_DESC tUIDesc;
		wstring wsOffTexProtoTag;
		wstring wsOffTexComTag;
		wstring wsOnTexProtoTag;
		wstring wsOnTexComTag;
		wstring wsPushTexProtoTag;
		wstring wsPushTexComTag;
		void(*func)();

		bool bTextOut;
	}UI_BUTTON_ADD_COMPONENT;
}

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
	void OnPlayerInfo(PLAYER_INFO* _pPlayerDesc);
	//void OnPlayerInfo(PLAYER_INFO* _pPlayerDesc);
	void OffPlayerInfo();

public:
	// 무기 탄약 - 무기 탄약
	void OnWeaponAmmom(WEAPON_INFO* _pWeaponDesc);
	void OffWeaponAmmom();

public:
	//몬스터의 체력
	void OnMonsterBar(_int* _iMaxHP, _int* _iMinHP);

private:
	//내부 메서드
	HRESULT SetWeaponUIArray();

public:
	virtual void Free() override;

private:
	class CPlayerInfoUI* m_pPlayerInfoUI = nullptr;
	class CWeaponAmmoInfoUI* m_pWeaponAmmoInfoUI = nullptr;
	class CLoadingBar* m_pHUD_HpBar = nullptr;
	class CLoadingBar* m_pHUD_ManaBar = nullptr;
	class CLoadingBar* m_pHUD_AmmoBar = nullptr;
	class CHUDTopUI* m_pHUD_TopUI = nullptr;

public:


private:
//#pragma region Test_Player
//	_uint m_iMaxs[3];
//	_uint m_iMins[3];
//#pragma endregion
//
//#pragma region Test_MonsterHP
//	_uint m_iMax;
//	_uint m_iMin;
//#pragma endregion

	LPDIRECT3DDEVICE9	m_pDevice;
};

#define __UIMANAGER_H__
#endif