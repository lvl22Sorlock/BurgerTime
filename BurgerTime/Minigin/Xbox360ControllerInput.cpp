
//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "Xbox360ControllerInput.h"

//-------------------------------------------------------------------------
//	Implementation Includes
//-------------------------------------------------------------------------
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

#include <unordered_map>
#include <memory>
//

using namespace inputEnums;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	Exceptions
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include <exception>
class invalidJoystickException : public std::exception
{
public:
	explicit invalidJoystickException()
		: std::exception("invalid joystick while trying to get controller joystick value")
	{}
};


#pragma region Implementation
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	Implementation Class
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

class Xbox360ControllerInput::Xbox360ControllerInputImpl final
{
public:
	Xbox360ControllerInputImpl(DWORD userIdx);
	~Xbox360ControllerInputImpl() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	Xbox360ControllerInputImpl(const Xbox360ControllerInputImpl& other) = delete;
	Xbox360ControllerInputImpl(Xbox360ControllerInputImpl&& other) noexcept = delete;
	Xbox360ControllerInputImpl& operator=(const Xbox360ControllerInputImpl& other) = delete;
	Xbox360ControllerInputImpl& operator=(Xbox360ControllerInputImpl&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	bool Update();
	bool IsUp(ControllerButton button, const XINPUT_GAMEPAD* pGamePadState = nullptr) const;
	bool IsDown(ControllerButton button, const XINPUT_GAMEPAD* pGamePadState = nullptr) const;
	bool WasUp(ControllerButton button) const;
	bool WasDown(ControllerButton button) const;
	bool IsPressed(ControllerButton button) const;
	bool IsReleased(ControllerButton button) const;
	bool IsJoystick(ControllerJoystickDirection joystick) const;
	Vector2<float> GetJoystick(ControllerJoystick joystick) const;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
	std::unordered_map<unsigned int, ControllerButton> m_ControllerButtonValues;

	XINPUT_STATE m_CurrentInputState;
	XINPUT_STATE m_PreviousInputState;
	const DWORD m_USER_IDX;
};

Xbox360ControllerInput::Xbox360ControllerInputImpl::Xbox360ControllerInputImpl(DWORD userIdx)
	: m_CurrentInputState{}
	, m_PreviousInputState{}
	, m_USER_IDX{userIdx}
{
	m_ControllerButtonValues[XINPUT_GAMEPAD_A]					= ControllerButton::ButtonA;
	m_ControllerButtonValues[XINPUT_GAMEPAD_B]					= ControllerButton::ButtonB;
	m_ControllerButtonValues[XINPUT_GAMEPAD_X]					= ControllerButton::ButtonX;
	m_ControllerButtonValues[XINPUT_GAMEPAD_Y]					= ControllerButton::ButtonY;
	m_ControllerButtonValues[XINPUT_GAMEPAD_DPAD_LEFT]			= ControllerButton::DPadRight;
	m_ControllerButtonValues[XINPUT_GAMEPAD_DPAD_RIGHT]			= ControllerButton::DPadLeft;
	m_ControllerButtonValues[XINPUT_GAMEPAD_DPAD_UP]			= ControllerButton::DPadDown;
	m_ControllerButtonValues[XINPUT_GAMEPAD_DPAD_DOWN]			= ControllerButton::DPadUp;
	m_ControllerButtonValues[XINPUT_GAMEPAD_LEFT_SHOULDER]		= ControllerButton::LeftShoulder;
	m_ControllerButtonValues[XINPUT_GAMEPAD_RIGHT_SHOULDER]		= ControllerButton::RightShoulder;
}

bool Xbox360ControllerInput::Xbox360ControllerInputImpl::Update()
{
	m_PreviousInputState = m_CurrentInputState;
	ZeroMemory(&m_CurrentInputState, sizeof(XINPUT_STATE));
	DWORD dwResult{ XInputGetState(m_USER_IDX, &m_CurrentInputState) };
	if (dwResult == ERROR_SUCCESS) // if succesful
	{
		return true;
	}	
	return false;
}


Vector2<float> Xbox360ControllerInput::Xbox360ControllerInputImpl::GetJoystick(ControllerJoystick joystick) const
{
	XINPUT_GAMEPAD gamePadState{ m_CurrentInputState.Gamepad };

	switch (joystick)
	{
	case ControllerJoystick::left:
		return Vector2<float>(gamePadState.sThumbLX, gamePadState.sThumbLY);
		break;
	case ControllerJoystick::right:
		return Vector2<float>(gamePadState.sThumbRX, gamePadState.sThumbRY);
		break;
	default:
		throw invalidJoystickException();
	}
}

bool Xbox360ControllerInput::Xbox360ControllerInputImpl::IsJoystick(ControllerJoystickDirection joystick) const
{
	XINPUT_GAMEPAD gamePadState{ m_CurrentInputState.Gamepad };

	switch (joystick)
	{
	case ControllerJoystickDirection::leftLeft:
		return gamePadState.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	case ControllerJoystickDirection::leftRight:
		return gamePadState.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	case ControllerJoystickDirection::leftDown:
		return gamePadState.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	case ControllerJoystickDirection::leftUp:
		return gamePadState.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ControllerJoystickDirection::rightLeft:
		return gamePadState.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	case ControllerJoystickDirection::rightRight:
		return gamePadState.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	case ControllerJoystickDirection::rightDown:
		return gamePadState.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	case ControllerJoystickDirection::rightUp:
		return gamePadState.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	default:
		return false;
	}
}

bool  Xbox360ControllerInput::Xbox360ControllerInputImpl::IsUp(ControllerButton button, const XINPUT_GAMEPAD* pGamePadState) const
{
	XINPUT_GAMEPAD gamePadState{ m_CurrentInputState.Gamepad };
	if (pGamePadState != nullptr)
		gamePadState = *pGamePadState;
	BYTE triggerValue{};
	ControllerButton pressedButton{};
	const unsigned int buttonValue{ static_cast<unsigned>(gamePadState.wButtons) };
	switch (button)
	{
	case ControllerButton::ButtonA:
	case ControllerButton::ButtonB:
	case ControllerButton::ButtonX:
	case ControllerButton::ButtonY:
	case ControllerButton::DPadLeft:
	case ControllerButton::DPadRight:
	case ControllerButton::DPadUp:
	case ControllerButton::DPadDown:
		// if inputmanager doesn't know what this button is
		if (m_ControllerButtonValues.find(buttonValue) == m_ControllerButtonValues.end()) return false;

		// if a button has been pressed and that is the button that we are checking for, return true
		pressedButton = m_ControllerButtonValues.at(buttonValue);
		if (pressedButton == button)
			return true;

		return false;
		break;
	case ControllerButton::LeftShoulder:
		triggerValue = gamePadState.bLeftTrigger;
		if (triggerValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return true;
		break;
	case ControllerButton::RightShoulder:
		triggerValue = gamePadState.bRightTrigger;
		if (triggerValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool  Xbox360ControllerInput::Xbox360ControllerInputImpl::IsDown(ControllerButton button, const XINPUT_GAMEPAD* pGamePadState) const
{
	// because down = !up
	// a normal button can't be half-down, it's either up or down
	return !IsUp(button, pGamePadState);
}

bool Xbox360ControllerInput::Xbox360ControllerInputImpl::WasUp(ControllerButton button) const
{
	const XINPUT_GAMEPAD GAME_PAD_STATE = m_PreviousInputState.Gamepad;
	return IsUp(button, &GAME_PAD_STATE);
}

bool Xbox360ControllerInput::Xbox360ControllerInputImpl::WasDown(ControllerButton button) const
{
	return !WasUp(button);
}

bool  Xbox360ControllerInput::Xbox360ControllerInputImpl::IsPressed(ControllerButton button) const
{
	return IsDown(button) && WasUp(button);
}

bool  Xbox360ControllerInput::Xbox360ControllerInputImpl::IsReleased(ControllerButton button) const
{
	return IsUp(button) && WasDown(button);
}


#pragma endregion


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	Xbox360ControllerInput Class
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
Xbox360ControllerInput::Xbox360ControllerInput(DWORD userIdx)
	: m_pImpl{ new Xbox360ControllerInputImpl(userIdx) }
{}

Xbox360ControllerInput::~Xbox360ControllerInput()
{
	delete m_pImpl;
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

bool Xbox360ControllerInput::Update() {
	return m_pImpl->Update();
}

bool Xbox360ControllerInput::IsUp(ControllerButton button) const {
	return m_pImpl->IsUp(button);
}

bool Xbox360ControllerInput::IsDown(ControllerButton button) const {
	return m_pImpl->IsDown(button);
}

bool Xbox360ControllerInput::IsPressed(ControllerButton button) const {
	return m_pImpl->IsPressed(button);
}

bool Xbox360ControllerInput::IsReleased(ControllerButton button) const {
	return m_pImpl->IsReleased(button);
}

bool Xbox360ControllerInput::IsJoystick(ControllerJoystickDirection joystickDirection) const
{
	return m_pImpl->IsJoystick(joystickDirection);
}

Vector2<float> Xbox360ControllerInput::GetJoystick(ControllerJoystick joystick) const
{
	return m_pImpl->GetJoystick(joystick);
}