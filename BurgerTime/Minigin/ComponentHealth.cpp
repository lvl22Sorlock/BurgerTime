#include "ComponentHealth.h"


ComponentHealth::ComponentHealth(dae::GameObject* pParent, int maxHealth)
	: ComponentBase(pParent)
	, m_MaxHealth{maxHealth}
	, m_CurrentHealth{maxHealth}
{

}


int ComponentHealth::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

int ComponentHealth::GetMaxHealth() const
{
	return m_MaxHealth;
}

void ComponentHealth::DealDamage(int damageToDeal)
{
	m_CurrentHealth -= damageToDeal;
	if (m_CurrentHealth <= 0)
	{
		m_CurrentHealth = 0;
		Die();
		return;
	}
	if (m_CurrentHealth > m_MaxHealth)
		m_CurrentHealth = m_MaxHealth;

	NotifyObservers(Observer::Event::playerHealthChanged);
}

void ComponentHealth::Die()
{
	NotifyObservers(Observer::Event::playerDied);
}
