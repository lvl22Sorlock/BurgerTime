#pragma once

#include "ComponentBase.h"

class ComponentTimer final: public ComponentBase
{
public:
	ComponentTimer(dae::GameObject* pParent);
	virtual ~ComponentTimer() = default;

	virtual void Update(float deltaTime) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	float GetCurrentFPS() const;
	float GetTotalTime() const;
	int GetTotalFrames() const;
	float GetAverageFPS() const;
	int GetCurrentFPSRounded() const;

	ComponentTimer(const ComponentTimer& other) = delete;
	ComponentTimer(ComponentTimer&& other) noexcept = delete;
	ComponentTimer& operator=(const ComponentTimer& other) = delete;
	ComponentTimer& operator=(ComponentTimer&& other)	noexcept = delete;

private:
	virtual std::size_t GetTypeHash() override;

	void UpdateCurrentFPS(float deltaTime);

	float m_CurrentFPS;
	float m_TotalTime;
	int m_TotalFrames;
	float m_AverageFPS;

	const float m_FPS_BUFFER_DURATION;
	float m_TimeSinceBufferSwitch;
	std::pair<int, float> m_FPSBufferNr1;
	std::pair<int, float> m_FPSBufferNr2;
	std::pair<int, float> m_FPSBufferNr3;
};

