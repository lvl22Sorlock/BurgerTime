#pragma once
#include "ComponentBase.h"
class ComponentText;
class ComponentHealth;
#include "Observer.h"

class ComponentHealthDisplay final : public ComponentBase, public Observer
{
public:
	ComponentHealthDisplay(dae::GameObject* pParent, const std::string& extraDisplayText = "Lives: ");
	ComponentHealthDisplay(dae::GameObject* pParent, dae::GameObject* pHealthComponentSource, const std::string& extraDisplayText = "Lives: ");
	~ComponentHealthDisplay() = default;

	//void Update(float deltaTime) override;
	void OnNotify(Observer::Event event, void* pData = nullptr) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	ComponentHealthDisplay(const ComponentHealthDisplay& other) = delete;
	ComponentHealthDisplay(ComponentHealthDisplay&& other) noexcept = delete;
	ComponentHealthDisplay& operator=(const ComponentHealthDisplay& other) = delete;
	ComponentHealthDisplay& operator=(ComponentHealthDisplay&& other)	noexcept = delete;
private:
	virtual std::size_t GetTypeHash() override;

	void Initialize();
	void Initialize(dae::GameObject* pHealthComponentSource);

	ComponentText* m_pTextComponent;
	ComponentHealth* m_pHealthComponent;
	std::string m_ExtraText;
};

