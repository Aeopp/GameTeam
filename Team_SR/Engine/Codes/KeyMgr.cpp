#include "KeyMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

bool CKeyMgr::Key_Pressing(int _key)
{

	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Up(int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

void CKeyMgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}

void CKeyMgr::Free()
{
}
