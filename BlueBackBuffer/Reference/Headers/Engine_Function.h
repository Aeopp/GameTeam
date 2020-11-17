#pragma once

#ifndef __ENGINE_FUNCTION_H__

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
_uint SafeAddRef(T& ptr)
{
	unsigned int iReferenceCount = 0;

	if (ptr)
	{
		iReferenceCount = ptr->AddRef();
	}

	return iReferenceCount;
}

template <typename T>
_uint SafeRelease(T& ptr)
{
	unsigned int iReferenceCount = 0;

	if (ptr)
	{
		iReferenceCount = ptr->Release();

		if (0 == iReferenceCount)
		{
			ptr = nullptr;
		}
	}

	return iReferenceCount;
}

#define __ENGINE_FUNCTION_H__
#endif