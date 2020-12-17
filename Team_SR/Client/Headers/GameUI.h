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
	virtual HRESULT RenderGameObject() override;

private:
	HRESULT	AddComponent();

public:
	virtual CGameObject * Clone(void * pArg = nullptr) PURE;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;	//�ϴ� ���߿�

protected:
	_matrix m_matOrthographic;
	_matrix m_matWorld;
	_matrix m_matView;

	_vector m_vUISize;
	_vector m_vUIPos;

};

