#pragma once
#include "..\Headers\GameObject.h"

USING(Engine)
class CGameUI abstract : public CGameObject
{
public:
	explicit CGameUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CGameUI() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT ReadyGameObjectPrototype() PURE;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) PURE;
	virtual _uint UpdateGameObject(float fDeltaTime) PURE;
	virtual _uint LateUpdateGameObject(float fDeltaTime) PURE;
	virtual HRESULT RenderGameObject() override PURE;

public:
	void SetShownBarUI() { m_bShown = true; }//ui��Ÿ����
	void SetInvisibleBarUI() { m_bShown = false; }//ui�����

private:
	HRESULT	AddComponent();

protected:
	_uint SetupUIMatrix(UI_DESC& _desc);


public:
	virtual CGameObject * Clone(void * pArg = nullptr) PURE;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

protected:
	UI_DESC m_UIDesc;
	bool m_bShown = true;
};

