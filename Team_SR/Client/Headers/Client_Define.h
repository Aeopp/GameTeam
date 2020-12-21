#pragma once

#ifndef __CLIENT_DEFINE_H__

#define WINCX 1920
#define WINCY 1080

#define TERRAIN_VTX_COUNTX 129
#define TERRAIN_VTX_COUNTZ 129


#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&) = delete;		\
	ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* Get_Instance();			\
	static _uint Destroy_Instance();			\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::Get_Instance()			\
{												\
	if (nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
_uint ClassName::Destroy_Instance()				\
{												\
	_uint iRefCnt = 0;							\
	if (m_pInstance)							\
	{											\
		iRefCnt = m_pInstance->Release();		\
	}											\
	return iRefCnt;								\
}

#define __CLIENT_DEFINE_H__
#endif