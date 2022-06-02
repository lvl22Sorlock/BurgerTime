#include "MiniginPCH.h"
#include "InputManager.h"

#include "../imgui-1.87/imgui.h"
#include "../imgui-1.87/imgui_impl_opengl2.h"
#include "../imgui-1.87/imgui_impl_sdl.h"


#include "Command.hpp"
#include <string>

using namespace dae;

//InputManager* InputManager::pInstance{ nullptr };
//InputManager* InputManager::GetInstance()
//{
//	return pInstance;
//}


InputManager::InputManager()
	:m_CurrentInputState{}
	,m_PreviousInputState{}
{
	m_SDLPressedKeys.reserve(50);
	m_SDLReleasedKeys.reserve(50);
}

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
	m_PreviousInputState = m_CurrentInputState;
	ZeroMemory(&m_CurrentInputState, sizeof(XINPUT_STATE));
	DWORD dwResult{ XInputGetState(0, &m_CurrentInputState) };
	if (dwResult == ERROR_SUCCESS) // if succesfull
	{
	}
	else // on fail
	{
		m_CurrentInputState = {};
		//return false;
	}
		SDL_Event e;
		//size_t pressedKeyNrIdx{0};
		//size_t releasedKeyNrIdx{ 0 };
		for (std::pair<const char, bool>& isKeyPressed : m_SDLPressedKeys)
		{
			isKeyPressed.second = false;
		}
		for (std::pair<const char, bool>& isKeyReleased : m_SDLReleasedKeys)
		{
			isKeyReleased.second = false;
		}

		while (SDL_PollEvent(&e))
		{
			//if (pressedKeyNrIdx >= m_SDLPressedKeys.size())			
			//	m_SDLPressedKeys.reserve(m_SDLPressedKeys.size() * 1.5f);
			//if (releasedKeyNrIdx >= m_SDLReleasedKeys.size())
			//	m_SDLReleasedKeys.reserve(m_SDLReleasedKeys.size() * 1.5f);

			switch (e.type)
			{
			case SDL_QUIT:
				return false;
				//break;
			case SDL_KEYDOWN:
				m_SDLPressedKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
				m_SDLDownKeys	[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
				break;
			case SDL_KEYUP:
				m_SDLReleasedKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
				m_SDLDownKeys	 [GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			}
	
			//process event for IMGUI
			ImGui_ImplSDL2_ProcessEvent(&e);
		}
		//if (pressedKeyNrIdx >= m_SDLPressedKeys.size())
		//	m_SDLPressedKeys.reserve(m_SDLPressedKeys.size() * 1.5f);
		//if (releasedKeyNrIdx >= m_SDLReleasedKeys.size())
		//	m_SDLReleasedKeys.reserve(m_SDLReleasedKeys.size() * 1.5f);

		//m_SDLPressedKeys[pressedKeyNrIdx] = static_cast<int>(SDL_QUIT);
		//m_SDLReleasedKeys[releasedKeyNrIdx] = static_cast<int>(SDL_QUIT);

		//return true;

	return true;
	//return false;
}

//bool InputManager::IsPressed(ControllerButton buttonToCheck) const
//{
//	const XINPUT_GAMEPAD gamePadState = m_CurrentInputState.Gamepad;
//	const unsigned int buttonValue = static_cast<unsigned>(gamePadState.wButtons);
//	const unsigned int previousButtonValue = static_cast<unsigned>((m_PreviousInputState.Gamepad).wButtons);
//
//	 if button wasn't released
//	if (!(buttonValue == previousButtonValue)) return false;
//
//	 if inputmanager doesn't know what this button is
//	if (m_ControllerButtonValues.find(buttonValue) == m_ControllerButtonValues.end()) return false;
//
//	ControllerButton pressedButton = m_ControllerButtonValues.at(buttonValue);
//	if (pressedButton == buttonToCheck)
//		return true;
//
//	// Keyboard
//	 if button wasn't released not necessary
//
//
//	return false;
//}

void InputManager::InitializeMaps()	// to actually use this there should be public functions to do this
{
	m_ControllerButtonValues[XINPUT_GAMEPAD_A] = ControllerButton::ButtonA;
	m_ControllerButtonValues[XINPUT_GAMEPAD_B] = ControllerButton::ButtonB;
	m_ControllerButtonValues[XINPUT_GAMEPAD_X] = ControllerButton::ButtonX;
	m_ControllerButtonValues[XINPUT_GAMEPAD_Y] = ControllerButton::ButtonY;

	//m_CommandButtons[ControllerButton::ButtonA] = JumpCommand();
	//m_CommandButtons[ControllerButton::ButtonB] = DuckCommand();
	//m_CommandButtons[ControllerButton::ButtonX] = FireCommand();
	//m_CommandButtons[ControllerButton::ButtonY] = FartCommand();
}

void InputManager::HandleInput()
{
	// Check Controller Input
	for (const std::pair<unsigned int, ControllerButton>& mapElement : m_ControllerButtonValues)
	{
		if (IsButtonPressed(mapElement.second))
		{
			if (m_ControllerCommandButtons.find(mapElement.second) != m_ControllerCommandButtons.end())
			{
				m_ControllerCommandButtons[mapElement.second]->Execute();
			}
		}
	}

	// Check Keyboard Input
	for (const std::pair<IsInputKeyboard, Command*>& mapElement : m_KeyboardCommandButtons)
	{
		switch (mapElement.first.inputType)
		{
		case InputType::IsDown:
			if (IsButtonDown(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsUp:
			if (IsButtonUp(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsPressed:
			if (IsButtonPressed(mapElement.first.key))
				mapElement.second->Execute();
			break;
		case InputType::IsReleased:
			if (IsButtonReleased(mapElement.first.key))
				mapElement.second->Execute();
			break;
		default:
			break;
		}
	}
}

bool InputManager::IsButtonUp(ControllerButton button) const
{
	const XINPUT_GAMEPAD gamePadState = m_CurrentInputState.Gamepad;
	const unsigned int buttonValue = static_cast<unsigned>(gamePadState.wButtons);

	// if inputmanager doesn't know what this button is
	if (m_ControllerButtonValues.find(buttonValue) == m_ControllerButtonValues.end()) return false;

	// if a button has been pressed and that is the button that we are checking for, return true
	ControllerButton pressedButton = m_ControllerButtonValues.at(buttonValue);
	if (pressedButton == button)
		return true;

	return false;
}

bool InputManager::IsButtonDown(ControllerButton button) const
{
	// because down = !up
	// a normal button can't be half-down, it's either up or down
	return !IsButtonUp(button);
}

bool InputManager::WasButtonUp(ControllerButton button) const
{
	const XINPUT_GAMEPAD gamePadState = m_PreviousInputState.Gamepad;
	const unsigned int buttonValue = static_cast<unsigned>(gamePadState.wButtons);

	// if inputmanager doesn't know what this button is
	if (m_ControllerButtonValues.find(buttonValue) == m_ControllerButtonValues.end()) return false;

	// if a button has been pressed and that is the button that we are checking for, return true
	ControllerButton pressedButton = m_ControllerButtonValues.at(buttonValue);
	if (pressedButton == button)
		return true;

	return false;
}

bool InputManager::WasButtonDown(ControllerButton button) const
{
	return !WasButtonUp(button);
}

bool InputManager::IsButtonPressed(ControllerButton button) const
{
	return IsButtonDown(button) && WasButtonUp(button);
}

bool InputManager::IsButtonReleased(ControllerButton button) const
{
	return IsButtonUp(button) && WasButtonDown(button);
}

bool InputManager::IsButtonUp(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLDownKeys.find(capitalButton) };
	return (buttonIterator == m_SDLDownKeys.end()
			||
			(*buttonIterator).second == false);
}

bool InputManager::IsButtonDown(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLDownKeys.find(capitalButton) };
	if (buttonIterator == m_SDLDownKeys.end()) return false;
	return (*buttonIterator).second;
}

bool InputManager::IsButtonPressed(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLPressedKeys.find(capitalButton) };
	if (buttonIterator == m_SDLPressedKeys.end()) return false;
	return (*buttonIterator).second;
}

bool InputManager::IsButtonReleased(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLReleasedKeys.find(capitalButton) };
	if (buttonIterator == m_SDLReleasedKeys.end()) return false;
	return (*buttonIterator).second;
}


bool InputManager::AddCommandToButton(ControllerButton button, Command* pNewCommand)
{
	if (m_ControllerCommandButtons.contains(button))
		return false;

	m_ControllerCommandButtons[button] = pNewCommand;
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

bool InputManager::AddButtonToValue(unsigned int value, ControllerButton button)
{
	if (m_ControllerButtonValues.contains(value))
		return false;

	m_ControllerButtonValues[value] = button;
	return true;
}

void InputManager::AddCommandToCommandList(Command* pNewCommand)
{
	m_CommandPtrs.emplace_back(pNewCommand);
}

char InputManager::GetKeyChar(int scanCode) const
{
	const int NR_A_SDL{		4/*static_cast<int>(SDL_SCANCODE_A)*/ };
	const int NR_A_CHAR{	static_cast<int>('A')};
	return static_cast<char>(scanCode + (NR_A_CHAR - NR_A_SDL));
}

char InputManager::GetCapitalChar(char possiblyCapitalChar) const
{
	possiblyCapitalChar = static_cast<char>(std::toupper(possiblyCapitalChar));

	/*if (possiblyCapitalChar < 'A' || possiblyCapitalChar > 'Z')
	{
		possiblyCapitalChar = static_cast<char>(static_cast<int>(possiblyCapitalChar) + (static_cast<int>('a') - static_cast<int>('A')));
	}*/
	return possiblyCapitalChar;
}
