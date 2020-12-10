#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
{
}

_uint CBase::AddRef()
{
	return m_iReferenceCount++;
}

_uint CBase::Release()
{
	if (0 == m_iReferenceCount)
	{
		Free();
		delete this;
		return 0;
	}

	return m_iReferenceCount--;
}



