#include "Time_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER)); 
	ZeroMemory(&m_StartTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_GoalTime, sizeof(LARGE_INTEGER));

}

void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceCounter(&m_StartTime);
	QueryPerformanceCounter(&m_GoalTime);
	QueryPerformanceFrequency(&m_CPUCount);
}

float CTime_Manager::Update_Time_Manager()
{
	QueryPerformanceFrequency(&m_CPUCount);
	QueryPerformanceCounter(&m_GoalTime);
	float fDeltaTime = float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart; 
	m_StartTime.QuadPart = m_GoalTime.QuadPart;

	return fDeltaTime;
}

void CTime_Manager::Free()
{
}
