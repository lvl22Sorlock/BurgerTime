#include "BurgerTimePCH.h"
#include "ComponentScoreDisplay.h"
#include "GameObject.h"
#include <typeinfo>
#include "ComponentText.h"
#include "ComponentScoreManager.h"
#include <string>

ComponentScoreDisplay::ComponentScoreDisplay(dae::GameObject* pParent, const std::string& extraDisplayText)
	: ComponentBase(pParent)
	, m_ExtraText{extraDisplayText}
{
	Initialize();
}

ComponentScoreDisplay::ComponentScoreDisplay(dae::GameObject* pParent, dae::GameObject* pScoreComponentSource, const std::string& extraDisplayText)
	: ComponentBase(pParent)
	, m_ExtraText{ extraDisplayText }
{
	Initialize(pScoreComponentSource);
}

void ComponentScoreDisplay::Initialize()
{

	m_pScoreManagerComponent = m_pParentGameObject->GetComponentPtr<ComponentScoreManager>();
	m_pTextComponent = m_pParentGameObject->GetComponentPtr<ComponentText>();

	if (!m_pScoreManagerComponent || !m_pTextComponent)
		throw;
}

void ComponentScoreDisplay::Initialize(dae::GameObject* pScoreComponentSource)
{
	m_pScoreManagerComponent = pScoreComponentSource->GetComponentPtr<ComponentScoreManager>();
	m_pTextComponent = m_pParentGameObject->GetComponentPtr<ComponentText>();

	if (!m_pScoreManagerComponent || !m_pTextComponent)
		throw;
}

//void ComponentScoreDisplay::Update(float)
//{
//	
//}

void ComponentScoreDisplay::OnNotify(Observer::Event event, void*)
{
	if (!m_pScoreManagerComponent || !m_pTextComponent)
		throw;

	if (event == Observer::Event::scoreGained)
	{
		m_pTextComponent->SetText("Score: " + std::to_string(m_pScoreManagerComponent->GetCurrentScore()));
		std::cout << "Score display changed" << std::endl;
	}
}
