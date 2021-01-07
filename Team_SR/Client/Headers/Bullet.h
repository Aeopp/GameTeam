#pragma once
#ifndef __BULLET_H__

#include "GameObject.h"
USING(Engine)
class CBullet abstract : public CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBullet() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
public:
	virtual void MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)override;
	void	Bullet_Attack();
	bool Attack(const Sphere _Sphere, const float Attack) &;
protected:
	virtual HRESULT AddComponents();
protected:
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;
protected:
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	float m_fFrameSpeed;	// 프레임 속도
	_vector m_vLook;		// 방향
	CTexture* m_pTexture;	// 텍스처
	BulletStatus m_stOriginStatus;	// 총알 원본 스텟
	BulletStatus m_stStatus;		// 총알 스텟
	bool m_bOneHit = false;
	_uint m_uiCountRelay;		// 특정 총알 전용 전달 카운트 변수
public:
	float Shine = 20.f;
	class CNormalUVVertexBuffer * _VertexBuffer{ nullptr };
};

#define  __BULLET_H__
#endif
