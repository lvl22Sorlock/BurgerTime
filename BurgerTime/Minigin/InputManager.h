#pragma once
//
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")
#pragma warning(pop)
#include <unordered_map>
#include <memory>
class Command;
//

#include "Singleton.h"
class KeyboardInput;
class Xbox360ControllerInput;
namespace inputEnums
{
	enum class ControllerButton;
	enum class ControllerJoystickDirection;
	enum class ControllerJoystick;
}

namespace dae
{
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
			: key{keyParam}
			, inputType{inputTypeParam}
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
	struct IsInputController
	{
		IsInputController(inputEnums::ControllerButton buttonParam, InputType inputTypeParam)
			: button{ buttonParam }
			, inputType{ inputTypeParam }
		{}

		inputEnums::ControllerButton button{};
		InputType inputType{ InputType::IsPressed };

		inline bool operator==(const IsInputController& other) const
		{
			return (button == other.button)
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
	struct HashIsInputController
	{
		std::size_t operator()(const IsInputController& key) const
		{
			return std::hash<int>()(static_cast<int>(key.button)) ^ std::hash<int>()(static_cast<int>(key.inputType));
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager();	// inputManager owns the commands

		void HandleInput();
		bool ProcessInput();		
		
		void AddCommandToCommandList(Command* pNewCommand);
		bool AddCommandToButton(inputEnums::ControllerButton button, InputType inputType, Command* pNewCommand, int playerNr);
		bool AddCommandToJoystickDirection(inputEnums::ControllerJoystickDirection joystickDirection, Command* pNewCommand, int playerNr);
		bool AddCommandToButton(char button, InputType inputType, Command* pNewCommand);
	private:
		friend class Singleton<InputManager>;
		InputManager();

		// Controller
		std::unique_ptr<Xbox360ControllerInput> m_pXbox360ControllerInputU1;
		std::unique_ptr<Xbox360ControllerInput> m_pXbox360ControllerInputU2;
		// Keyboard
		std::unique_ptr<KeyboardInput> m_pKeyboardInput;
		char GetCapitalChar(char possiblyCapitalChar) const;

		std::vector<Command*> m_CommandPtrs;
		std::unordered_map<IsInputController, Command*, HashIsInputController> m_ControllerCommandButtonsU1;
		std::unordered_map<IsInputController, Command*, HashIsInputController> m_ControllerCommandButtonsU2;
		std::unordered_map<inputEnums::ControllerJoystickDirection, Command*> m_ControllerCommandJoystickDirectionsU1;
		std::unordered_map<inputEnums::ControllerJoystickDirection, Command*> m_ControllerCommandJoystickDirectionsU2;
		std::unordered_map<IsInputKeyboard, Command*, HashIsInputKeyboard> m_KeyboardCommandButtons;
	};
}


