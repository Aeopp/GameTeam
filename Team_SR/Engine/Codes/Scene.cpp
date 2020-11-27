#include "..\Headers\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	SafeAddRef(pDevice);
}

HRESULT CScene::ReadyScene()
{
	return S_OK;
}

_uint CScene::UpdateScene()
{
	return _uint();
}

_uint CScene::LateUpdateScene()
{
	return _uint();
}

void CScene::Free()
{
	SafeRelease(m_pDevice);
}




//class CBase
//{
//public:
//	CBase(int i)
//	{
//
//	}
//};
//
//void Func(int i);
//void Func(CBase obj);
//
//void main()
//{
//	Func(100); CBase obj = 100;
//	Func(CBase(100)); CBase obj = CBase(100);
//}

