#pragma once
#include "ComponentBase.h"
class ComponentText;
class ComponentScoreManager;
#include "Observer.h"

class ComponentScoreDisplay final : public ComponentBase, public Observer
{
public:
	ComponentScoreDisplay(dae::GameObject* pParent, const std::string& extraDisplayText = "Score: ");
	ComponentScoreDisplay(dae::GameObject* pParent, dae::GameObject* pScoreComponentSource, const std::string& extraDisplayText = "Score: ");
	~ComponentScoreDisplay() = default;

	void OnNotify(Event event, void* pData = nullptr) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	virtual std::size_t GetTypeHash() override;

	ComponentScoreDisplay(const ComponentScoreDisplay& other) = delete;
	ComponentScoreDisplay(ComponentScoreDisplay&& other) noexcept = delete;
	ComponentScoreDisplay& operator=(const ComponentScoreDisplay& other) = delete;
	ComponentScoreDisplay& operator=(ComponentScoreDisplay&& other)	noexcept = delete;
private:

	void Initialize();
	void Initialize(dae::GameObject* pScoreComponentSource);

	ComponentText* m_pTextComponent;
	ComponentScoreManager* m_pScoreManagerComponent;
	std::string m_ExtraText;
};

