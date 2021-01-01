#pragma once
#include "GameUI.h"
#include "UIManager.h"


USING(UI_AddTag)
class CButtonUI final : public CGameUI
{
private:
	enum class EButtonState
	{
		DISABLED,
		ACTIVATION,
		PUSH,
		BUTTON_END,
	};

private:
	CButtonUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CButtonUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:	//
	HRESULT	AddComponent(UI_BUTTON_ADD_COMPONENT* _pInputTag);

public:
	static CButtonUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	//class CTexture* m_pTextureCom = nullptr; //부모의 텍스쳐는 비활성화를 기준으로
	class CTexture* m_pOnTextureCom = nullptr;
	class CTexture* m_pPushTextureCom = nullptr;

private:
	class CKeyMgr* m_pKeyMgr;
	float m_fMaxSize;
	bool m_bTextOut;
	wstring m_wsObjectName;
	RECT m_tBoxRect;

	EButtonState m_eState = EButtonState::DISABLED;
	//함수 포인터를 저장할 변수
	void(*m_function)() = nullptr;
};
