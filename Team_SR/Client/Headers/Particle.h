#pragma once
#ifndef __PARTICLE_H__

#include "GameObject.h"
USING(Engine)
class CParticle final : public CGameObject
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CParticle() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	HRESULT AddComponents();

private:
	bool Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	HRESULT IsBillboarding();	// 빌보드
public:
	static CParticle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
private:
	class CVIBuffer* m_pVIBufferCom = nullptr;

private:
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	CTexture* m_pTexture;	// 텍스처
	EFFECT m_eType;			// 파티클 종류
	bool m_bFrameLoopCheck;	// 프레임 루프
};

#define  __PARTICLE_H__
#endif
