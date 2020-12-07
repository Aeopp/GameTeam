#pragma once

#ifndef __KEYMGR_H__

#include "Base.h"

#define VK_MAX 0xff

BEGIN(Engine)
class ENGINE_DLL CKeyMgr final : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	CKeyMgr();
	virtual ~CKeyMgr() = default;

public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);
	void Key_Update();

public:
	virtual void Free() override;

private:
	bool				m_bKeyState[VK_MAX];
};
END

#define __KEYMGR_H__
#endif // !__KEYMGR_H__
