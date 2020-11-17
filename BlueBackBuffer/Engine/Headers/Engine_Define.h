#pragma once

#ifndef __ENGINE_DEFINE_H__

#ifdef ENGINE_EXPORT
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define PRINT_LOG(caption, message)	\
::MessageBox(0, message, caption, MB_OK);


#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&) = delete;		\
	ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* Get_Instance();			\
	static void Destroy_Instance();				\
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
void ClassName::Destroy_Instance()				\
{												\
	if (m_pInstance)							\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

#define __ENGINE_DEFINE_H__
#endif