#include "MiniginPCH.h"
#include "InputManager.h"

#include "../imgui-1.87/imgui.h"
#include "../imgui-1.87/imgui_impl_opengl2.h"
#include "../imgui-1.87/imgui_impl_sdl.h"


#include "Command.hpp"
#include <string>

using namespace dae;

#include "KeyboardInput.h"
#include "Xbox360ControllerInput.h"
#include "StructsEnums.h"
using namespace inputEnums;

InputManager::InputManager()
	: m_pKeyboardInput{std::make_unique<KeyboardInput>()}
	, m_pXbox360ControllerInputU1{ std::make_unique<Xbox360ControllerInput>(1) }
	, m_pXbox360ControllerInputU2{ std::make_unique<Xbox360ControllerInput>(2) }
	, m_ControllerCommandButtonsU1{}
	, m_ControllerCommandButtonsU2{}
	, m_ControllerCommandJoystickDirectionsU1{}
	, m_ControllerCommandJoystickDirectionsU2{}
	, m_KeyboardCommandButtons{}
{}

InputManager::~InputManager()
{
	for (Command* pCommand : m_CommandPtrs)
	{
		delete pCommand;
	}
	m_CommandPtrs.clear();
}

bool dae::InputManager::ProcessInput()
{
	m_pKeyboardInput->Update();
	m_pXbox360ControllerInputU1->Update();
	m_pXbox360ControllerInputU2->Update();
	return true;
}

void InputManager::HandleInput()
{
	// Check Controller Input
	for (const std::pair<IsInputController, Command*>& buttonCommandPair : m_ControllerCommandButtonsU1)
	{
		switch (buttonCommandPair.first.inputType)
		{
		case InputType::IsDown:
			if (m_pXbox360ControllerInputU1->IsDown(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsUp:
			if (m_pXbox360ControllerInputU1->IsUp(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsPressed:
			if (m_pXbox360ControllerInputU1->IsPressed(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsReleased:
			if (m_pXbox360ControllerInputU1->IsReleased(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		default:
			break;
		}
	}
	for (const std::pair<IsInputController, Command*>& buttonCommandPair : m_ControllerCommandButtonsU2)
	{
		switch (buttonCommandPair.first.inputType)
		{
		case InputType::IsDown:
			if (m_pXbox360ControllerInputU2->IsDown(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsUp:
			if (m_pXbox360ControllerInputU2->IsUp(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsPressed:
			if (m_pXbox360ControllerInputU2->IsPressed(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		case InputType::IsReleased:
			if (m_pXbox360ControllerInputU2->IsReleased(buttonCommandPair.first.button))
				buttonCommandPair.second->Execute();
			break;
		default:
			break;
		}
	}
	for (const std::pair<ControllerJoystickDirection, Command*>& joystickDirectionCommandPair : m_ControllerCommandJoystickDirectionsU1)
	{
		if (m_pXbox360ControllerInputU1->IsJoystick(joystickDirectionCommandPair.first))
		{
			joystickDirectionCommandPair.second->Execute();
		}
	}
	for (const std::pair<ControllerJoystickDirection, Command*>& joystickDirectionCommandPair : m_ControllerCommandJoystickDirectionsU2)
	{
		if (m_pXbox360ControllerInputU2->IsJoystick(joystickDirectionCommandPair.first))
		{
			joystickDirectionCommandPair.second->Execute();
		}
	}

	// Check Keyboard Input
	for (const std::pair<IsInputKeyboard, Command*>& mapElement : m_KeyboardCommandButtons)
	{
		switch (mapElement.first.inputType)
		{
		case InputType::IsDown:
			if (m_pKeyboardInput->IsDown(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsUp:
			if (m_pKeyboardInput->IsUp(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsPressed:
			if (m_pKeyboardInput->IsPressed(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsReleased:
			if (m_pKeyboardInput->IsReleased(mapElement.first.key))
				mapElement.second->Execute();
			break;
		default:
			break;
		}
	}
}

bool InputManager::AddCommandToButton(ControllerButton button, InputType inputType, Command* pNewCommand, int playerNr)
{
	if (playerNr != 1 && playerNr != 2) {
		std::cout << "Invalid playerNr for controller input: " << playerNr << std::endl;
		return false;
	}
	IsInputController isInputController{ button, inputType };

	auto* pControllerButtonCommandMap{ &m_ControllerCommandButtonsU1};
	if (playerNr == 2)
		pControllerButtonCommandMap = &m_ControllerCommandButtonsU2;

	if (pControllerButtonCommandMap->contains(isInputController))
		return false;

	(*pControllerButtonCommandMap)[isInputController] = pNewCommand;
	return true;
}

bool InputManager::AddCommandToJoystickDirection(inputEnums::ControllerJoystickDirection joystickDirection, Command* pNewCommand, int playerNr)
{
	if (playerNr != 1 && playerNr != 2) {
		std::cout << "Invalid playerNr for controller input: " << playerNr << std::endl;
		return false;
	}
	auto* pControllerButtonCommandMap{ &m_ControllerCommandJoystickDirectionsU1 };
	if (playerNr == 2)
		pControllerButtonCommandMap = &m_ControllerCommandJoystickDirectionsU2;

	if (pControllerButtonCommandMap->contains(joystickDirection))
		return false;

	(*pControllerButtonCommandMap)[joystickDirection] = pNewCommand;
	return true;
}

bool InputManager::AddCommandToButton(char button, InputType inputType, Command* pNewCommand)
{
	IsInputKeyboard isInputKeyboard(GetCapitalChar(button), inputType);
	if (m_KeyboardCommandButtons.contains(isInputKeyboard))
		return false;

	m_KeyboardCommandButtons[isInputKeyboard] = pNewCommand;
	return true;
}

void InputManager::AddCommandToCommandList(Command* pNewCommand)
{
	m_CommandPtrs.emplace_back(pNewCommand);
}

char InputManager::GetCapitalChar(char possiblyCapitalChar) const
{
	possiblyCapitalChar = static_cast<char>(std::toupper(possiblyCapitalChar));

	return possiblyCapitalChar;
}
