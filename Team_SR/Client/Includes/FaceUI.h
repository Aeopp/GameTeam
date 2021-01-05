#pragma once
#include "GameUI.h"
class CFaceUI final : public CGameUI
{
private:
	enum class EFaceState
	{
		UNHURT,
		BLEEDING,
		INJURED,
		DYING,
		FACE_END,
	};

private:
	explicit CFaceUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFaceUI() = default;

public:
	// CGameUI을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	HRESULT	AddComponent();

public:
	static CFaceUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	//CGameUI::m_pTextureCom : Unhurt Texture
	class CTexture* m_pBleedingTexCom = nullptr;
	class CTexture* m_pInjuredTexCom = nullptr;
	class CTexture* m_pDyingTexCom = nullptr;

	EFaceState m_eFaceState;
	float m_fAniTime;
};

