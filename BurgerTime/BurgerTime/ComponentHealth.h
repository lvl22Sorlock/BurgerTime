#pragma once
#include "ComponentBase.h"
#include "Subject.h"

class ComponentHealth final : public ComponentBase, public Subject
{
public:
	ComponentHealth(dae::GameObject* pParent, int maxHealth);
	~ComponentHealth() = default;

	int GetCurrentHealth() const;
	int GetMaxHealth() const;
	void DealDamage(int damageToDeal);
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	ComponentHealth(const ComponentHealth& other) = delete;
	ComponentHealth(ComponentHealth&& other) noexcept = delete;
	ComponentHealth& operator=(const ComponentHealth& other) = delete;
	ComponentHealth& operator=(ComponentHealth&& other)	noexcept = delete;
private:
	virtual std::size_t GetTypeHash() override;

	void Die();

	int m_MaxHealth;
	int m_CurrentHealth;
};

