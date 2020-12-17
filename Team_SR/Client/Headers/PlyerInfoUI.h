#pragma once

#ifndef __PLAYERINFOUI_H__
#include "..\Headers\GameObject.h"

USING(Engine)
class CPlyerInfoUI final : public CGameObject
{
private:
	explicit CPlyerInfoUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlyerInfoUI() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:	//���θ޼������
	HRESULT	AddComponent();

public:
	static CPlyerInfoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:	//������Ʈ
	class CVIBuffer* m_pVIBufferCom = nullptr; 
	class CTexture* m_pTextureCom = nullptr;	//�ϴ� ���߿�

private:


};

#define __PLAYERINFOUI_H__
#endif // !__PLAYERINFOUI_H__
