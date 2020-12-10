#pragma once
#ifndef __BASE_H__
#include "Engine_Include.h"
BEGIN(Engine)
class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	_uint AddRef();
	_uint Release();

protected:
	virtual void Free() = 0;
protected:
	_uint m_iReferenceCount = 0;
};
END

#define __BASE_H__
#endif

