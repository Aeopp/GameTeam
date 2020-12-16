#pragma once
#ifndef __SCENE_H__

#include "Base.h"
#include "KeyMgr.h"
BEGIN(Engine)
class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

public:
	virtual HRESULT ReadyScene() = 0;
	virtual _uint UpdateScene(float fDeltaTime) = 0;
	virtual _uint LateUpdateScene() = 0;

public:
	// 2020.12.16 21:32 KMJ
	// 해당 씬의 인덱스를 가져옵니다
	_int GetSceneIndex() { return m_iSceneIndex; };

protected:
	virtual _uint KeyProcess(float fDeltaTime);
public:
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9		m_pDevice;
	CKeyMgr*				m_pKeyMgr;
	class CManagement*		m_pManagement;
	// 2020.12.16 21:32 KMJ
	// 씬 인덱스
	// 게임 오브젝트 클론 예약 생성할 때 필요함
	// 객체들은 현제 씬 인덱스를 모름, 매니지먼트가 씬매니저를 통해 알아오게됨
	_int					m_iSceneIndex;
};
END

#define __SCENE_H__
#endif