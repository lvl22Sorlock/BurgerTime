#include "BurgerTimePCH.h"
#include "ComponentHealthDisplay.h"
#include "GameObject.h"
#include <typeinfo>
#include "ComponentText.h"
#include "ComponentHealth.h"
#include <string>

std::size_t ComponentHealthDisplay::GetTypeHash()
{
	return typeid(*this).hash_code();
}


ComponentHealthDisplay::ComponentHealthDisplay(dae::GameObject* pParent, const std::string& extraDisplayText)
	: ComponentBase(pParent)
	, m_ExtraText{extraDisplayText}
{
	Initialize();
}

ComponentHealthDisplay::ComponentHealthDisplay(dae::GameObject* pParent, dae::GameObject* pHealthComponentSource, const std::string& extraDisplayText)
	: ComponentBase(pParent)
	, m_ExtraText{ extraDisplayText }
{
	Initialize(pHealthComponentSource);
}

void ComponentHealthDisplay::Initialize()
{
	m_pHealthComponent	= m_pParentGameObject->GetComponentPtr<ComponentHealth>();
	m_pTextComponent	= m_pParentGameObject->GetComponentPtr<ComponentText>();

	if (!m_pHealthComponent || !m_pTextComponent)
		throw;
}

void ComponentHealthDisplay::Initialize(dae::GameObject* pHealthComponentSource)
{
	m_pHealthComponent = pHealthComponentSource->GetComponentPtr<ComponentHealth>();
	m_pTextComponent = m_pParentGameObject->GetComponentPtr<ComponentText>();

	if (!m_pHealthComponent || !m_pTextComponent)
		throw;
}

void ComponentHealthDisplay::OnNotify(Observer::Event event, void*)
{
	if (!m_pHealthComponent || !m_pTextComponent)
		throw;

	if (event == Observer::Event::playerHealthChanged)
	{
		m_pTextComponent->SetText(m_ExtraText + std::to_string(m_pHealthComponent->GetCurrentHealth()));
		std::cout << "Health display changed" << std::endl;
	}
	else if (event == Observer::Event::playerDied)
	{
		m_pTextComponent->SetText(m_ExtraText + std::to_string(m_pHealthComponent->GetCurrentHealth()) + " (DEAD)");
		std::cout << "Player has died, Health display changed" << std::endl;
	}
}
