#pragma once

#include "Engine_Include.h"

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

