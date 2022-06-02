#include "ComponentScoreManager.h"







ComponentScoreManager::ComponentScoreManager(dae::GameObject* pParent)
	: ComponentBase(pParent)
	, m_CurrentScore{0}
{

}


int ComponentScoreManager::GetCurrentScore() const
{
	return m_CurrentScore;
}

void ComponentScoreManager::IncreaseScore(int scoreToAdd)
{
	if (scoreToAdd <= 0) return;

	m_CurrentScore += scoreToAdd;

	NotifyObservers(Observer::Event::scoreGained);
}
