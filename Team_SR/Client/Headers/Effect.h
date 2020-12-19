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
	bool Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	CTexture* m_pTexture;	// 텍스처
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	float m_fFrameSpeed;					// 프레임 속도
	bool m_bFrameLoopCheck;					// 프레임 루프
};

#endif // Effect_h__