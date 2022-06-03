#pragma once
#include "ComponentBase.h"
#include "Subject.h"

class ComponentScoreManager final : public ComponentBase, public Subject
{
public:
	ComponentScoreManager(dae::GameObject* pParent);
	~ComponentScoreManager() = default;

	int GetCurrentScore() const;
	void IncreaseScore(int scoreToAdd);

	ComponentScoreManager(const ComponentScoreManager& other) = delete;
	ComponentScoreManager(ComponentScoreManager&& other) noexcept = delete;
	ComponentScoreManager& operator=(const ComponentScoreManager& other) = delete;
	ComponentScoreManager& operator=(ComponentScoreManager&& other)	noexcept = delete;
private:
	virtual std::size_t GetTypeHash() override;

	int m_CurrentScore;
};

