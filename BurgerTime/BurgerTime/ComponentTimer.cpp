#include "BurgerTimePCH.h"
#include "ComponentTimer.h"

std::size_t ComponentTimer::GetTypeHash()
{
	return typeid(*this).hash_code();
}

ComponentTimer::ComponentTimer(dae::GameObject* pParent)
	: ComponentBase(pParent)
	, m_CurrentFPS{}
	, m_TotalTime{}
	, m_TotalFrames{}
	, m_AverageFPS{}
	, m_FPS_BUFFER_DURATION{0.33f}
	, m_TimeSinceBufferSwitch{0.0f}
	, m_FPSBufferNr1{ std::pair<int,float>(0,0.f) }
	, m_FPSBufferNr2{ std::pair<int,float>(0,0.f) }
	, m_FPSBufferNr3{ std::pair<int,float>(0,0.f) }
{

}

void ComponentTimer::Update(float deltaTime)
{
	UpdateCurrentFPS(deltaTime);

	++m_TotalFrames;
	m_TotalTime += deltaTime;
	m_AverageFPS = m_TotalFrames / m_TotalTime;

}

float ComponentTimer::GetCurrentFPS() const
{
	return m_CurrentFPS;
}

float ComponentTimer::GetTotalTime() const
{
	return m_TotalTime;
}

int ComponentTimer::GetTotalFrames() const
{
	return m_TotalFrames;
}

float ComponentTimer::GetAverageFPS() const
{
	return m_AverageFPS;
}

int ComponentTimer::GetCurrentFPSRounded() const
{
	return static_cast<int>(m_CurrentFPS + 0.5f);	// would need to be -0.5f if m_CurrentFPS is negative, but this should never happen
}

void ComponentTimer::UpdateCurrentFPS(float deltaTime)
{
	{
		++m_FPSBufferNr1.first;
		m_FPSBufferNr1.second += deltaTime;
		if (m_FPSBufferNr1.second >= m_FPS_BUFFER_DURATION)
		{
			m_FPSBufferNr3 = m_FPSBufferNr2;
			m_FPSBufferNr2 = m_FPSBufferNr1;
			m_FPSBufferNr1 = std::pair<int,float>( 0,0.f );
		}
	}

	if (m_FPSBufferNr2.first == 0) m_CurrentFPS = m_FPSBufferNr1.first / m_FPSBufferNr1.second;
	if (m_FPSBufferNr3.first == 0)
		m_CurrentFPS = (m_FPSBufferNr1.first + m_FPSBufferNr2.first) / (m_FPSBufferNr1.second + m_FPSBufferNr2.second);

	m_CurrentFPS = 
		(m_FPSBufferNr1.first + m_FPSBufferNr2.first + m_FPSBufferNr3.first) 
		/ 
		(m_FPSBufferNr1.second + m_FPSBufferNr2.second + m_FPSBufferNr3.second);
}
