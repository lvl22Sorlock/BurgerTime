#pragma once
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

#include <unordered_map>
#include <memory>
class Command;
//

#include "Singleton.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};
	enum class InputType
	{
		IsPressed,
		IsReleased,
		IsDown,
		IsUp
	};
	struct IsInputKeyboard
	{
		IsInputKeyboard(char keyParam, InputType inputTypeParam)
			:key{keyParam}
			,inputType{inputTypeParam}
		{}

		char key{};
		InputType inputType{ InputType::IsPressed };

		inline bool operator==(const IsInputKeyboard& other) const
		{
			return (key == other.key)
				&&
				(inputType == other.inputType);
		}
	};
	// https://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
	struct HashIsInputKeyboard
	{
		std::size_t operator()(const IsInputKeyboard& key) const
		{
			return std::hash<char>()(key.key) ^ std::hash<int>()(static_cast<int>(key.inputType));
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();	// inputManager owns the commands

		void HandleInput();
		bool ProcessInput();		
		
		void AddCommandToCommandList(Command* pNewCommand);
		bool AddCommandToButton(ControllerButton button, Command* pNewCommand);
		bool AddCommandToButton(char button, InputType inputType, Command* pNewCommand);
	private:
		// Controller
		bool IsButtonUp(ControllerButton button) const;
		bool IsButtonDown(ControllerButton button) const;
		bool WasButtonUp(ControllerButton button) const;
		bool WasButtonDown(ControllerButton button) const;
		bool IsButtonPressed(ControllerButton button) const;
		bool IsButtonReleased(ControllerButton button) const;
		// Keyboard
		char GetKeyChar(int scanCode) const;
		char GetCapitalChar(char possiblyCapitalChar) const;
		bool IsButtonUp(char button) const;
		bool IsButtonDown(char button) const;
		bool IsButtonPressed(char button) const;
		bool IsButtonReleased(char button) const;

		void InitializeMaps();
		bool AddButtonToValue(unsigned int value, ControllerButton button);

		XINPUT_STATE m_CurrentInputState{};		
		XINPUT_STATE m_PreviousInputState;
		std::unordered_map<char, bool> m_SDLReleasedKeys;
		std::unordered_map<char, bool> m_SDLPressedKeys;
		std::unordered_map<char, bool> m_SDLDownKeys;

		BYTE m_ByteMaxValue;
		SHORT m_ShortMaxValue;
		float m_ValuePercentageToIgnore;

		std::vector<Command*> m_CommandPtrs;
		std::unordered_map<unsigned int, ControllerButton> m_ControllerButtonValues;
		std::unordered_map<ControllerButton, Command*> m_ControllerCommandButtons;
		std::unordered_map<IsInputKeyboard, Command*, HashIsInputKeyboard> m_KeyboardCommandButtons;
	};

}


