#pragma once
#ifndef __DECORATOR_H__

#include "GameObject.h"
USING(Engine)
class CDecorator final : public CGameObject
{
private:
	explicit CDecorator(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDecorator() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
private:
	HRESULT AddComponents();

public:
	virtual void Hit(CGameObject * const _Target, const Collision::Info & _CollisionInfo) override;	// 장식이 피해를 받음
	virtual void ParticleHit(void* const _Particle, const Collision::Info& _CollisionInfo)override;
private:
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	HRESULT IsBillboarding();	// 빌보드
public:
	static CDecorator* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
private:
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	float m_fTriggerHP;		// 다음 프레임 구간으로 넘어갈 체력
	DecoratorInfo m_stDecoratorInfo;	// 장식 정보
	list<DecoNextFrameInfo> m_listNextFrameInfo;	// 다음 프레임 구간 정보들
	CTexture* m_pTexture;	// 텍스처
	class CNormalUVVertexBuffer* _VertexBuffer{ nullptr };

	void UpdateFromMyDecoType();
};

#define  __DECORATOR_H__
#endif
