#pragma once
#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
USING(Engine)
class CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
protected:
	virtual HRESULT AddComponents();
	HRESULT IsBillboarding();
protected:
	bool Frame_Move(float fDeltaTime);		// �ؽ�ó ������ �̵�
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	CTexture* m_pTexture;	// �ؽ�ó
	float m_fFrameCnt;		// ������ ��ȣ
	float m_fStartFrame;	// ������ ����
	float m_fEndFrame;		// ������ ��
	float m_fFrameSpeed;					// ������ �ӵ�
	bool m_bFrameLoopCheck;					// ������ ����
};

#endif // Effect_h__