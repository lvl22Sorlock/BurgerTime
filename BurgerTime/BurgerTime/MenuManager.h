#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
#include "StructsEnums.h"
#include "Command.hpp"
namespace dae
{
	class GameObject;
	class Minigin;
	class Scene;
}


//-------------------------------------------------------------------------
//	MenuManager Class
//-------------------------------------------------------------------------
class MenuManager final : public dae::Singleton<MenuManager>
{
public:
	friend class dae::Singleton<MenuManager>;
	~MenuManager() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	MenuManager(const MenuManager& other) = delete;
	MenuManager(MenuManager&& other) noexcept = delete;
	MenuManager& operator=(const MenuManager& other) = delete;
	MenuManager& operator=(MenuManager&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void GoToMainMenu();
	void GoToPauseMenu();
	void TogglePauseMenu();
	void LeaveMenu();

	void SetScene(dae::Scene* pScene);
	void SetEngine(dae::Minigin* pScene);
	

private:
	MenuManager();
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------
	void LeaveMainMenu();
	void LeavePauseMenu();

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	bool m_IsMainMenu;
	dae::GameObject* m_pMainMenuObject;
	bool m_IsPauseMenu;
	dae::GameObject* m_pPauseMenuObject;
	dae::Scene* m_pScene;
	dae::Minigin* m_pEngine;

public:
//class LeaveMainMenuCommand : public Command
//{
//public:
//	LeaveMainMenuCommand()
//		:Command()
//	{}
//	~LeaveMainMenuCommand() = default;
//
//	void Execute() override
//	{
//		MenuManager::GetInstance().LeaveMainMenu();
//	}
//};
class TogglePauseMenuCommand : public Command
{
public:
	TogglePauseMenuCommand()
		:Command()
	{}
	~TogglePauseMenuCommand() = default;

	void Execute() override
	{
		MenuManager::GetInstance().TogglePauseMenu();
	}
};
};


