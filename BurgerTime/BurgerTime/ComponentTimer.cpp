#include "BurgerTimePCH.h"
#include "ComponentTimer.h"

ComponentTimer::ComponentTimer(dae::GameObject* pParent)
	: ComponentBase(pParent)
	, m_CurrentFPS{}
	, m_TotalTime{}
	, m_TotalFrames{}
	, m_AverageFPS{}
{

}

void ComponentTimer::Update(float deltaTime)
{
	++m_TotalFrames;
	m_TotalTime += deltaTime;

	m_CurrentFPS = 1 / deltaTime;
	m_AverageFPS = m_TotalFrames / m_TotalTime;

}
void ComponentTimer::Render() const
{

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
