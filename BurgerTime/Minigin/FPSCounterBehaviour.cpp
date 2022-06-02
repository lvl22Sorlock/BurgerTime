#include "MiniginPCH.h"
#include "FPSCounterBehaviour.h"
#include "GameObject.h"
#include "ComponentTimer.h"
#include "ComponentText.h"

FPSCounterBehaviour::FPSCounterBehaviour(dae::GameObject* pParent)
	:ComponentBase(pParent)
	,MIN_TIME_BETWEEN_FPS_UPDATES{0.15f}
	,m_TimeSinceLastFPSUpdate{0}
{
	Initialize();
}

void FPSCounterBehaviour::Initialize()
{
	//ComponentBase* pTimerComponent{ nullptr };
	//pTimerComponent = m_pParentGameObject->GetComponentPtr(typeid(ComponentTimer)/*"Timer"*/);
	//if (!pTimerComponent)
	//{
	//	std::cout << "FPSCounterBehaviour component does not have access to a ComponentTimer" << std::endl;
	//	return;
	//}
	//ComponentBase* pTextComponent{ nullptr };
	//pTextComponent = m_pParentGameObject->GetComponentPtr(typeid(ComponentText)/*"Text"*/);
	//if (!pTextComponent)
	//{		
	//	std::cout << "FPSCounterBehaviour component does not have access to a ComponentText" << std::endl;
	//	return;
	//}

	//m_pTimerComponent = dynamic_cast<ComponentTimer*>(pTimerComponent);
	//m_pTextComponent = dynamic_cast<ComponentText*>(pTextComponent);
	m_pTimerComponent = m_pParentGameObject->GetComponentPtr<ComponentTimer>();
	m_pTextComponent = m_pParentGameObject->GetComponentPtr<ComponentText>();

	//if (!m_pTextComponent)
	//	std::cout << "FPSCounterBehaviour component does not have access to a ComponentText" << std::endl;

	//if (!m_pTimerComponent)
	//	std::cout << "FPSCounterBehaviour component does not have access to a ComponentTimer" << std::endl;

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