#pragma once

#ifndef __ENGINE_FUNCTION_H__
#include <string>

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
		ptr = nullptr;
	}

	return iReferenceCount;
}

static std::wstring ToWstring(const std::string& Lhs)
{
	std::wstring Rhs;
	Rhs.assign(std::begin(Lhs), std::end(Lhs));
	return Rhs;
}

static std::string ToString(const std::wstring& Lhs)
{
	std::string Rhs;
	Rhs.assign(std::begin(Lhs), std::end(Lhs));
	return Rhs;
}

template<typename _Type>
static std::wstring TYPE_NAME()
{
	std::string _TypeS = typeid(_Type).name();
	std::wstring _TypeW;

	if (_TypeS.find("class C") != std::string::npos)
	{
		_TypeS =		_TypeS.substr(_TypeS.find_first_of('C')+1);
	}
	else if (_TypeS.find("class ") != std::string::npos)
	{
		_TypeS = _TypeS.substr(_TypeS.find_first_of(' ')+1);
	}

	_TypeW.assign(std::make_move_iterator(std::begin(_TypeS)),
				std::make_move_iterator(std::end(_TypeS)));

	return _TypeW;
}

#define __ENGINE_FUNCTION_H__
#endif