//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "MenuManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Minigin.h"
#include "GameObject.h"
#include "ComponentSpriteRenderer.h"
#include "BurgerTimeGlobal.h"
#include "Scene.h"

using namespace dae;
using namespace SimonGlobalConstants;

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
MenuManager::MenuManager()
	: m_IsMainMenu{ false }
	, m_IsPauseMenu{ false }
	, m_pMainMenuObject{ nullptr }
	, m_pPauseMenuObject{ nullptr }
	, m_pScene{ nullptr }
	, m_pEngine{ nullptr }
{}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void MenuManager::GoToMainMenu()
{
	if (m_pEngine == nullptr) return;
	if (m_pMainMenuObject == nullptr) return;
	if (m_pScene == nullptr) return;
	
	m_pScene->SetIsActiveAll(false);

	m_IsMainMenu = true;
	m_pEngine->SetTimePaused(true);
	m_pMainMenuObject->SetActive(true);
}
void MenuManager::GoToPauseMenu()
{
	if (m_pEngine == nullptr) return;
	if (m_pPauseMenuObject == nullptr) return;
	if (m_pScene == nullptr) return;

	m_pScene->SetIsActiveAll(false);

	m_IsPauseMenu = true;
	m_pEngine->SetTimePaused(true);
	m_pPauseMenuObject->SetActive(true);
}
void MenuManager::LeaveMainMenu()
{
	if (m_pEngine == nullptr) return;
	if (m_pMainMenuObject == nullptr) return;
	if (m_pScene == nullptr) return;

	m_pScene->SetIsActiveAll(true);
	m_pPauseMenuObject->SetActive(false);

	m_IsMainMenu = false;
	m_pEngine->SetTimePaused(false);
	m_pMainMenuObject->SetActive(false);
}
void MenuManager::LeavePauseMenu()
{
	if (m_pEngine == nullptr) return;
	if (m_pPauseMenuObject == nullptr) return;
	if (m_pScene == nullptr) return;

	m_pScene->SetIsActiveAll(true);
	m_pMainMenuObject->SetActive(false);

	m_IsPauseMenu = false;
	m_pEngine->SetTimePaused(false);
	m_pPauseMenuObject->SetActive(false);
}

void MenuManager::SetScene(Scene* pScene)
{
	m_pScene = pScene;

	if (m_pMainMenuObject == nullptr)
	{
		const auto pMainMenuObjectSharedPtr{ std::make_shared<GameObject>() };
		m_pMainMenuObject = pMainMenuObjectSharedPtr.get();
		ComponentSpriteRenderer* pMainMenuSpriteRenderer{
			new ComponentSpriteRenderer(
				m_pMainMenuObject,
				Vector2<float>(static_cast<float>(SCREEN_SIZE.x), static_cast<float>(SCREEN_SIZE.y)),
				false,
				{},
				{640, 480}
			)
		};
		pMainMenuSpriteRenderer->SetSpritesheet("MainMenu.png");
		m_pMainMenuObject->AddComponent(pMainMenuSpriteRenderer);
		*pMainMenuSpriteRenderer->GetIsMirroredPtr() = false;

		m_pMainMenuObject->SetActive(m_IsMainMenu);
		m_pScene->Add(pMainMenuObjectSharedPtr);
	}
	if (m_pPauseMenuObject == nullptr)
	{
		const auto pPauseMenuObjectSharedPtr{ std::make_shared<GameObject>() };
		m_pPauseMenuObject = pPauseMenuObjectSharedPtr.get();
		ComponentSpriteRenderer* pPauseMenuSpriteRenderer{
			new ComponentSpriteRenderer(
				m_pPauseMenuObject,
				Vector2<float>(static_cast<float>(SCREEN_SIZE.x), static_cast<float>(SCREEN_SIZE.y)),
				false,
				{},
				{640, 480}
			) 
		};
		pPauseMenuSpriteRenderer->SetSpritesheet("PauseMenu.png");
		m_pPauseMenuObject->AddComponent(pPauseMenuSpriteRenderer);
		*pPauseMenuSpriteRenderer->GetIsMirroredPtr() = false;

		m_pPauseMenuObject->SetActive(m_IsPauseMenu);
		m_pScene->Add(pPauseMenuObjectSharedPtr);
	}
}

void MenuManager::SetEngine(Minigin* pEngine)
{
	m_pEngine = pEngine;
}

void MenuManager::LeaveMenu()
{
	if (m_IsMainMenu)
		LeaveMainMenu();
	if (m_IsPauseMenu)
		LeavePauseMenu();
}

void MenuManager::TogglePauseMenu()
{
	if (m_IsMainMenu) {
		LeaveMainMenu();
		return;
	}

	if (m_IsPauseMenu)
		LeavePauseMenu();
	else
		GoToPauseMenu();
}