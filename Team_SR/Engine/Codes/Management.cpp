#include "..\Headers\Management.h"
#include "CollisionComponent.h"


USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pGraphic_Dev(CGraphic_Device::Get_Instance())
	, m_pTimeManager(CTime_Manager::Get_Instance())
	, m_pSceneManager(CSceneManager::Get_Instance())
	, m_pGameObjectManager(CGameObjectManager::Get_Instance())
	, m_pComponentManager(CComponentManager::Get_Instance())
{

#ifdef _DEBUG
	bDebug = true;
#endif // DEBUG

	SafeAddRef(m_pGraphic_Dev);
	SafeAddRef(m_pTimeManager);
	SafeAddRef(m_pSceneManager);
	SafeAddRef(m_pGameObjectManager);
	SafeAddRef(m_pComponentManager);
}

HRESULT CManagement::ReadyEngine(
	HWND hWnd, 
	_uint iWinCX, 
	_uint iWinCY, 
	EDisplayMode eDisplaymode,
	_uint iSceneCount)
{
	MATH::RandomInit();

	if (FAILED(m_pGraphic_Dev->Ready_Graphic_Device(hWnd, iWinCX, iWinCY, eDisplaymode)))
		return E_FAIL;

	if (FAILED(m_pGameObjectManager->ReserveSizeContainer(iSceneCount)))
		return E_FAIL;

	if (FAILED(m_pComponentManager->ReserveSizePrototypeContainer(iSceneCount)))
		return E_FAIL;

	m_pRenderer = CRenderer::Create(m_pGraphic_Dev->Get_Device());
	if (nullptr == m_pRenderer)
	{
		PRINT_LOG(L"Error", L"Failed To Create Renderer");
		return E_FAIL;
	}

	return S_OK;
}

_uint CManagement::UpdateEngine()
{
	if (nullptr == m_pSceneManager)
		return 0;

	/* For.DeltaTime */
	float fDeltaTime = m_pTimeManager->Update_Time_Manager();

	// 2020.12.16 16:27 KMJ
	// 예약된 처리
	if (FAILED(ScheduledProcessing())) {
		PRINT_LOG(L"Error", L"Failed To ScheduledProcessing");
	}

	/* For.Update */
	m_iUpdateEvent = m_pSceneManager->UpdateScene(fDeltaTime);
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return m_iUpdateEvent;

	m_iUpdateEvent = m_pGameObjectManager->UpdateGameObject(fDeltaTime);

	/* For.LateUpdate */
	m_iUpdateEvent = m_pSceneManager->LateUpdateScene();
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return m_iUpdateEvent;

	m_iUpdateEvent = m_pGameObjectManager->LateUpdateGameObject(fDeltaTime);

	CCollisionComponent::CollisionUpdate(m_pGraphic_Dev->Get_Device());
	
	return _uint();
}

HRESULT CManagement::RenderEngine(HWND hWnd)
{
	if (CHANGE_SCNENE == m_iUpdateEvent)
		return S_OK;

	if (nullptr == m_pRenderer)
		return E_FAIL;	

	return m_pRenderer->Render(hWnd);
}

HRESULT CManagement::ClearForScene(_int iSceneIndex)
{
	if (FAILED(m_pGameObjectManager->ClearForScene(iSceneIndex)))
		return E_FAIL;

	if (FAILED(m_pComponentManager->ClearForScene(iSceneIndex)))
		return E_FAIL;

	return S_OK;
}

LPDIRECT3DDEVICE9 CManagement::GetDevice()
{
	if (nullptr == m_pGraphic_Dev)
		return nullptr;

	return m_pGraphic_Dev->Get_Device();
}

LPDIRECT3D9 CManagement::GetSDK()
{
	if (nullptr == m_pGraphic_Dev)
		return nullptr;

	return m_pGraphic_Dev->Get_SDK();
}

D3DPRESENT_PARAMETERS& CManagement::GetD3Dpp()
{
	return m_pGraphic_Dev->GetD3Dpp();
}


HRESULT CManagement::SetUpCurrentScene(_int iSceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;

	CurrentSceneIdx = iSceneID;

	return m_pSceneManager->SetUpCurrentScene(iSceneID, pCurrentScene);
}

CGameObject * CManagement::GetGameObject(_int iSceneIndex, const wstring & LayerTag, _uint iIndex)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;

	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}

	return m_pGameObjectManager->GetGameObject(iSceneIndex, LayerTag, iIndex);
}

CComponent * CManagement::GetComponent(
	_int iSceneIndex, 
	const wstring& LayerTag, 
	const wstring& ComponentTag, 
	_uint iIndex)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;

	if (iSceneIndex == -1)
	{
		iSceneIndex = CurrentSceneIdx;
	}

	auto pGameObject = m_pGameObjectManager->GetGameObject(
		iSceneIndex, LayerTag, iIndex);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject->GetComponent(ComponentTag);
}

HRESULT CManagement::AddGameObjectPrototype(
	_int iSceneIndex, 
	const wstring & GameObjectTag, 
	CGameObject * pPrototype)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;

	return m_pGameObjectManager->AddGameObjectPrototype(iSceneIndex, GameObjectTag, pPrototype);
}

HRESULT CManagement::AddGameObjectInLayer(
	_int iFromSceneIndex, 
	const wstring& GameObjectTag, 
	_int iToSceneIndex, 
	const wstring& LayerTag, 
	CGameObject** ppGameObject,
	void* pArg)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;

	return m_pGameObjectManager->AddGameObjectInLayer(iFromSceneIndex, GameObjectTag, iToSceneIndex, LayerTag, ppGameObject, pArg);
}

// 2020.12.16 15:31 KMJ
// 예약된 게임 오브젝트 추가 - 다음 프레임 Update 전 처음에 생성됩니다
void CManagement::AddScheduledGameObjectInLayer(_int iFromSceneIndex, const wstring & GameObjectTag, _int iToSceneIndex, const wstring & LayerTag, CGameObject ** ppGameObject, void * pArg)
{
	ScheduledGameObjectInfo stScheduledObjInfo;
	stScheduledObjInfo.iFromSceneIndex = iFromSceneIndex;
	stScheduledObjInfo.wstrGameObjectTag = GameObjectTag;
	stScheduledObjInfo.iToSceneIndex = iToSceneIndex;
	stScheduledObjInfo.wstrLayerTag = LayerTag;
	stScheduledObjInfo.ppGameObject = ppGameObject;
	stScheduledObjInfo.pArg = pArg;
	m_listScheduledObjInfo.push_back(stScheduledObjInfo);
}

HRESULT CManagement::AddComponentPrototype(_int iSceneIndex, const wstring & ComponentTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponentManager)
		return E_FAIL;

	return m_pComponentManager->AddComponentPrototype(iSceneIndex, ComponentTag, pPrototype);
}

CComponent * CManagement::CloneComponentPrototype(
	_int iSceneIndex, 
	const wstring & ComponentTag, 
	void * pArg)
{
	if (nullptr == m_pComponentManager)
		return nullptr;

	return m_pComponentManager->CloneComponentPrototype(iSceneIndex, ComponentTag, pArg);
}

HRESULT CManagement::AddGameObjectInRenderer(ERenderID eID, CGameObject * pGameObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->AddGameObjectInRenderer(eID, pGameObject);
}

void CManagement::RegistLight(const D3DLIGHT9& Light)
{
	m_pRenderer->RegistLight(Light);
}

// 2020.12.16 16:44 KMJ
// 예정된 처리
HRESULT CManagement::ScheduledProcessing()
{
	HRESULT retVa = S_OK;
	for (auto& schedule : m_listScheduledObjInfo) {
		// 오브젝트 레이어에 추가
		if (FAILED(m_pGameObjectManager->AddGameObjectInLayer(
			schedule.iFromSceneIndex,
			schedule.wstrGameObjectTag,
			schedule.iToSceneIndex,
			schedule.wstrLayerTag,
			schedule.ppGameObject,
			schedule.pArg))) {
			retVa = E_FAIL;
		}
	}
	// 비우기
	m_listScheduledObjInfo.clear();

	return retVa;
}

void CManagement::Free()
{
	SafeRelease(m_pRenderer);
	SafeRelease(m_pGameObjectManager);
	SafeRelease(m_pComponentManager);
	SafeRelease(m_pSceneManager);
	SafeRelease(m_pTimeManager);
	SafeRelease(m_pGraphic_Dev);
}

void CManagement::ReleaseEngine()
{
	if(CManagement::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CManagement");

	if (CGameObjectManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGameObjectManager");

	if (CComponentManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CComponentManager");

	if (CSceneManager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CSceneManager");

	if (CTime_Manager::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CTime_Manager");

	if (CGraphic_Device::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGraphic_Device");
}
