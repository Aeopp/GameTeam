#pragma once
#ifndef __BATGREY_H__

#include "Monster.h"
USING(Engine)
class CBatGrey final : public CMonster
{
private:
	explicit CBatGrey(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBatGrey() = default;
public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;

private:
	virtual HRESULT AddComponents() override;
	
	void Frame_Move(float fDeltaTime);		// 텍스처 프레임 이동
	virtual HRESULT Set_Texture();			// 장치에 텍스처 셋

public:
	static CBatGrey* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	float m_fFrameCnt;						// 프레임 번호
	float m_fStartFrame;					// 프레임 시작
	float m_fEndFrame;						// 프레임 끝
	wstring m_wstrTextureKey;				// 텍스처 키
	map<wstring, CTexture*> m_mapTexture;	// 텍스처 맵
};

#define __GLACIER_H__
#endif