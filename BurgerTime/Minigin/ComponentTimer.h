#pragma once

#include "ComponentBase.h"

class ComponentTimer final: public ComponentBase
{
public:
	ComponentTimer(dae::GameObject* pParent);
	virtual ~ComponentTimer() = default;

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

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
	float m_CurrentFPS;
	float m_TotalTime;
	int m_TotalFrames;
	float m_AverageFPS;
};
