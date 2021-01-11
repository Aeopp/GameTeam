#pragma once
#ifndef __MANAGEMENT_H__

#include "Base.h"
#include "Graphic_Device.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "Time_Manager.h"
#include "Renderer.h"

BEGIN(Engine)
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement();
	virtual ~CManagement() = default;

public:
	/* For.General */
	HRESULT ReadyEngine(HWND hWnd, _uint iWinCX, _uint iWinCY, EDisplayMode eDisplaymode, _uint iSceneCount);
	_uint UpdateEngine();
	HRESULT RenderEngine(HWND hWnd = nullptr);
	HRESULT ClearForScene(_int iSceneIndex);
public:
	/* For.GraphicDev */
	LPDIRECT3DDEVICE9 GetDevice();
	LPDIRECT3D9 GetSDK();
	D3DPRESENT_PARAMETERS& GetD3Dpp();
public:
	/* For.SceneManager */
	HRESULT SetUpCurrentScene(_int iSceneID, CScene* pCurrentScene);
	CScene* GetCurrentScene() { return m_pSceneManager->GetCurrentScene(); };
public: /* For.GameObjectManager */
	CGameObject* GetGameObject(_int iSceneIndex, const wstring& LayerTag, _uint iIndex = 0);
	CComponent* GetComponent(_int iSceneIndex, const wstring& LayerTag, const wstring& ComponentTag, _uint iIndex = 0);
	FORCEINLINE _int GetCurrentSceneIndex()const& { return CurrentSceneIdx; };
	std::list<class CGameObject*> GetGameObjects(_int iSceneIndex, const wstring& LayerTag);

	HRESULT AddGameObjectPrototype(_int iSceneIndex, wstring GameObjectTag, CGameObject* pPrototype);
	HRESULT AddGameObjectInLayer(_int iFromSceneIndex, const wstring& GameObjectTag, _int iToSceneIndex, const wstring& LayerTag, CGameObject** ppGameObject = nullptr, void* pArg = nullptr);
	// 2020.12.16 15:31 KMJ
	// 예약된 게임 오브젝트 추가 - 다음 프레임 Update 전 처음에 생성됩니다
	void AddScheduledGameObjectInLayer(_int iFromSceneIndex, const wstring& GameObjectTag, const wstring& LayerTag, CGameObject** ppGameObject = nullptr, void* pArg = nullptr);
	// 2021.01.11 16:33 KMJ
	// 예약된 씬 교체 - 다음 프레임 Update 전 처음에 교체됩니다
	void AddScheduledReplaceScene(_int iSceneID, CScene* pCurrentScene);

public: /* For.ComponentManager */
	HRESULT AddComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, CComponent* pPrototype);
	CComponent* CloneComponentPrototype(_int iSceneIndex, const wstring& ComponentTag, void* pArg = nullptr);

public: /* For.Renderer */
	HRESULT AddGameObjectInRenderer(ERenderID eID, class CGameObject* pGameObject, ERenderPlace ePlace = ERenderPlace::BACK);
	void RegistLight(const D3DLIGHT9& Light);
	void SetAmbient(const DWORD Ambient) { m_pRenderer->SetAmbient(Ambient); };
	D3DCAPS9 GetCaps() { return m_pRenderer->GetCaps(); };
	void SetEffectRender(std::function<void()> _EffectRender) { m_pRenderer->_ParticleRender = std::move(_EffectRender); };
public:
	ID3DXLine& GetDXLine() { return m_pGraphic_Dev->GetLine(); };
private:
	// 2020.12.16 16:44 KMJ
	// 예약된 처리
	HRESULT ScheduledProcessing();
public:
	virtual void Free() override;
	static void ReleaseEngine();
	int32_t CurrentSceneIdx;
	bool bDebug = false;
	std::function<void(const float)> _ParticleUpdate;
	std::function<void(const float)> _ParticleLateUpdate;
	std::function<void()> _ParticleCollision;
	std::function<void()> _ParticleRender;
private:
	CGraphic_Device*	m_pGraphic_Dev = nullptr;
	CTime_Manager*		m_pTimeManager = nullptr;
	CSceneManager*		m_pSceneManager = nullptr;
	CGameObjectManager*	m_pGameObjectManager = nullptr;
	CComponentManager*	m_pComponentManager = nullptr;
	CRenderer*			m_pRenderer = nullptr;

	_uint m_iUpdateEvent = 0;

	list<ScheduledGameObjectInfo> m_listScheduledObjInfo;

	_int m_iReplaceSceneID = 0;
	CScene* m_pReplaceScene = nullptr;
};
END

#define __MANAGEMENT_H__
#endif