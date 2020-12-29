#pragma once
#ifndef __ITEM_H__

#include "GameObject.h"
USING(Engine)
class CItem final : public CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CItem() = default;
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
	ItemInfo GetItemInfo();		// 아이템 정보
private:
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	HRESULT IsBillboarding();	// 빌보드
public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void Hit(CGameObject* const _Target, const Collision::Info& _CollisionInfo);
	FORCEINLINE bool IsAcheive()const& { return bAcheive; };
private:
	float m_fFrameCnt;		// 프레임 번호
	float m_fStartFrame;	// 프레임 시작
	float m_fEndFrame;		// 프레임 끝
	CTexture* m_pTexture;	// 텍스처
	ItemInfo m_stItemInfo;	// 아이템 정보
	class CNormalUVVertexBuffer* _VertexBuffer{ nullptr };
	// 플레이어가 획득하였나요?
	bool bAcheive = false;
	float CreateAfterTime = 0.0f;
};

#define  __ITEM_H__
#endif
