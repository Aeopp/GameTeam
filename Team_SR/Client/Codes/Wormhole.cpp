#include "stdafx.h"
#include "..\Headers\Wormhole.h"
#include "Stage1st.h"
#include "Stage2nd.h"
#include "Stage3rd.h"
#include "Stage5th.h"
#include "Stage6th.h"
#include "StageMidBoss.h"
#include "StageBoss.h"


CWormhole::CWormhole(LPDIRECT3DDEVICE9 pDevice)
	:CBullet(pDevice)
{
}


HRESULT CWormhole::ReadyGameObjectPrototype()
{
	if (FAILED(CBullet::ReadyGameObjectPrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWormhole::ReadyGameObject(void* pArg /*= nullptr*/)
{
	if (FAILED(CBullet::ReadyGameObject(pArg)))
		return E_FAIL;

	if (FAILED(AddComponents()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (sizeof(WormholeArgument) == *static_cast<_uint*>(pArg)) {
			WormholeArgument* pArgument = static_cast<WormholeArgument*>(pArg);
			m_pTransformCom->m_TransformDesc.vPosition = pArgument->vPosition;
			m_eReplaceSceneID = pArgument->eReplaceSceneID;
			// 동적 생성된 거임
			if (pArgument->bDeleteFlag) {
				delete pArg;
			}
		}
	}

	m_pTransformCom->m_TransformDesc.vScale = { 2.5f,2.5f,2.5f };

	// 기본 텍스처 프레임
	m_fFrameCnt = 0;
	m_fStartFrame = 0;
	m_fEndFrame = 4;
	m_fFrameSpeed = 10.f;

	return S_OK;
}

_uint CWormhole::UpdateGameObject(float fDeltaTime)
{
	CBullet::UpdateGameObject(fDeltaTime);

	_CollisionComp->Update(m_pTransformCom);

	if (!m_bOneHit)
	{
		// 플레이어 충돌 처리
		if (Attack(_CollisionComp->_Sphere, 0))
		{
			m_bOneHit = true;
			
			CManagement* pManagement = CManagement::Get_Instance();
			if (nullptr == pManagement)
				return 0;

			CScene* pReplaceScene = nullptr;

			switch (m_eReplaceSceneID)
			{
			case ESceneID::CurrentScene:
				break;
			case ESceneID::Static:
				break;
			case ESceneID::Logo:
				break;
			case ESceneID::Menu:
				break;
			case ESceneID::Stage1st:
				pReplaceScene = CStage1st::Create(m_pDevice);
				break;
			case ESceneID::Stage2nd:
				pReplaceScene = CStage2nd::Create(m_pDevice);
				break;
			case ESceneID::Stage3rd:
				pReplaceScene = CStage3rd::Create(m_pDevice);
				break;
			case ESceneID::Stage4th:
				break;
			case ESceneID::Stage5th:
				pReplaceScene = CStage5th::Create(m_pDevice);
				break;
			case ESceneID::Stage6th:
				pReplaceScene = CStage6th::Create(m_pDevice);
				break;
			case ESceneID::Stage7th:
				break;
			case ESceneID::Stage8th:
				break;
			case ESceneID::StageMidBoss:
				pReplaceScene = CStageMidBoss::Create(m_pDevice);
				break;
			case ESceneID::StageFinalBoss:
				pReplaceScene = CStageBoss::Create(m_pDevice);
				break;
			case ESceneID::MaxCount:
				break;
			default:
				break;
			}

			pManagement->AddScheduledReplaceScene((_int)m_eReplaceSceneID, pReplaceScene);
		}
	}

	return _uint();
}

_uint CWormhole::LateUpdateGameObject(float fDeltaTime)
{
	CBullet::LateUpdateGameObject(fDeltaTime);

	if (FAILED(m_pManagement->AddGameObjectInRenderer(ERenderID::Alpha, this)))
		return 0;

	Frame_Move(fDeltaTime);	// 텍스처 프레임 이동

	return _uint();
}

HRESULT CWormhole::RenderGameObject()
{
	if (FAILED(CBullet::RenderGameObject()))
		return E_FAIL;

	return S_OK;
}

// 부모 클래스 Bullet의 호출을 막기 위함
void CWormhole::MapHit(const PlaneInfo & _PlaneInfo, const Collision::Info & _CollisionInfo)
{
}

void CWormhole::Frame_Move(float fDeltaTime)
{
	m_fFrameCnt += m_fFrameSpeed * fDeltaTime;
	if (m_fFrameCnt >= m_fEndFrame)
	{
		m_fFrameCnt = m_fStartFrame;
	}
}

HRESULT CWormhole::AddComponents()
{
	if (FAILED(CBullet::AddComponents()))	// Monster.cpp에서 RectTexture 호출
		return E_FAIL;

#pragma region Add_Component_Texture
	// 포탈 텍스처
	if (FAILED(CGameObject::AddComponent(
		(_int)ESceneID::Static,
		L"Component_Texture_Wormhole",
		L"Com_Texture_Wormhole",
		(CComponent**)&m_pTexture)))
		return E_FAIL;
#pragma endregion

	// 충돌 컴포넌트
	CCollisionComponent::InitInfo _Info;
	_Info.bCollision = true;
	_Info.bMapBlock = true;
	_Info.Radius = 1.f;
	_Info.Tag = CCollisionComponent::ETag::MonsterAttack;
	_Info.bWallCollision = true;
	_Info.bFloorCollision = true;
	_Info.Owner = this;
	CGameObject::AddComponent(
		static_cast<int32_t>(ESceneID::Static),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		CComponent::Tag + TYPE_NAME<CCollisionComponent>(),
		(CComponent**)&_CollisionComp, &_Info);

	return S_OK;
}

CWormhole* CWormhole::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWormhole* pInstance = new CWormhole(pDevice);
	if (FAILED(pInstance->ReadyGameObjectPrototype()))
	{
		PRINT_LOG(L"Warning", L"Failed To Create CWormhole");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject* CWormhole::Clone(void* pArg/* = nullptr*/)
{
	CWormhole* pClone = new CWormhole(*this); /* 복사생성자 */
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyGameObject(pArg)))
	{
		PRINT_LOG(L"Warning", L"Failed To Clone CWormhole");
		SafeRelease(pClone);
	}

	return pClone;
}

void CWormhole::Free()
{
	CBullet::Free();
}
