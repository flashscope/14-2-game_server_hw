#pragma once
#include "MacroSet.h"

class Timer : public Singleton<Timer>
{
public:
	Timer();
	~Timer();
	
	void	Tick();
	UINT	GetElapsedTime() const { return m_DeltaTime; }
	UINT	GetPrevTime() const { return m_PrevTime; }
	UINT	GetNowTime() const { return m_NowTime; }

private:
	UINT	m_PrevTime = 0;
	UINT	m_DeltaTime = 0;
	UINT	m_NowTime = 0;
};

