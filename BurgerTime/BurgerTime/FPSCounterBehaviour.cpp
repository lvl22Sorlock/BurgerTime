#include "BurgerTimePCH.h"
#include "FPSCounterBehaviour.h"
#include "GameObject.h"
#include "ComponentTimer.h"
#include "ComponentText.h"

std::size_t FPSCounterBehaviour::GetTypeHash()
{
	return typeid(*this).hash_code();
}

FPSCounterBehaviour::FPSCounterBehaviour(dae::GameObject* pParent)
	: ComponentBase(pParent)
	, MIN_TIME_BETWEEN_FPS_UPDATES{ 0.15f }
	, m_TimeSinceLastFPSUpdate{ 0 }
{
	Initialize();
}

void FPSCounterBehaviour::Initialize()
{
	m_pTimerComponent = m_pParentGameObject->GetComponentPtr<ComponentTimer>();
	m_pTextComponent = m_pParentGameObject->GetComponentPtr<ComponentText>();
}

void FPSCounterBehaviour::Update(float deltaTime)
{
	if (!m_pTextComponent || !m_pTimerComponent)
		return;

	m_TimeSinceLastFPSUpdate += deltaTime;
	if (m_TimeSinceLastFPSUpdate < MIN_TIME_BETWEEN_FPS_UPDATES) return;
	m_TimeSinceLastFPSUpdate = 0.0f;

	m_pTextComponent->SetText(std::to_string(m_pTimerComponent->GetCurrentFPSRounded()) + " FPS");
}