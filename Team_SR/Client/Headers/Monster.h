#pragma once
#ifndef __MONSTER_H__

#include "GameObject.h"
USING(Engine)
class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMonster() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() = 0;
	virtual HRESULT ReadyGameObject(void* pArg = nullptr) = 0;
	virtual _uint UpdateGameObject(float fDeltaTime) = 0;
	virtual _uint LateUpdateGameObject(float fDeltaTime) = 0;
	virtual HRESULT RenderGameObject() = 0;
protected:
	virtual HRESULT AddComponents();
protected:
	BOOL PlayerAwareness();		// 플레이어 인식 - 인식하면 TRUE, 인식하지 못하면 FALSE
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
protected:
	class CVIBuffer* m_pVIBufferCom = nullptr;

protected:
	float m_fFrameCnt;						// 프레임 번호
	float m_fStartFrame;					// 프레임 시작
	float m_fEndFrame;						// 프레임 끝
	CGameObject* m_pPlayer;					// 플레이어 포인터
	MonsterStatus m_stOriginStatus;			// 몬스터 원본 스텟
	MonsterStatus m_stStatus;				// 몬스터 스텟
	wstring m_wstrTextureKey;				// 텍스처 키
	map<wstring, CTexture*> m_mapTexture;	// 텍스처 맵
};

#define  __MONSTER_H__
#endif
