#pragma once

#include "ComponentBase.h"
class ComponentTimer;
class ComponentText;

class FPSCounterBehaviour final : public ComponentBase
{
public:
	FPSCounterBehaviour(dae::GameObject* pParent);
	virtual ~FPSCounterBehaviour() = default;

	virtual void Update(float deltaTime) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	FPSCounterBehaviour(const FPSCounterBehaviour& other) = delete;
	FPSCounterBehaviour(FPSCounterBehaviour&& other) noexcept = delete;
	FPSCounterBehaviour& operator=(const FPSCounterBehaviour& other) = delete;
	FPSCounterBehaviour& operator=(FPSCounterBehaviour&& other)	noexcept = delete;

private:
	virtual std::size_t GetTypeHash() override;

	void Initialize();

	const float MIN_TIME_BETWEEN_FPS_UPDATES;
	float m_TimeSinceLastFPSUpdate;

	ComponentTimer* m_pTimerComponent;
	ComponentText* m_pTextComponent;
};

