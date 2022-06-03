#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "StructsEnums.h"

//-------------------------------------------------------------------------
//	Xbox360ControllerInput Class
//-------------------------------------------------------------------------
class Xbox360ControllerInput final
{
public:	
	Xbox360ControllerInput(DWORD userIdx);
	~Xbox360ControllerInput();

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	Xbox360ControllerInput(const Xbox360ControllerInput& other) = delete;
	Xbox360ControllerInput(Xbox360ControllerInput&& other) noexcept = delete;
	Xbox360ControllerInput& operator=(const Xbox360ControllerInput& other) = delete;
	Xbox360ControllerInput& operator=(Xbox360ControllerInput&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	bool Update();
	bool IsUp(inputEnums::ControllerButton button) const;
	bool IsDown(inputEnums::ControllerButton button) const;
	bool IsPressed(inputEnums::ControllerButton button) const;
	bool IsReleased(inputEnums::ControllerButton button) const;
	bool IsJoystick(inputEnums::ControllerJoystickDirection joystickDirection) const;
	Vector2<float> GetJoystick(inputEnums::ControllerJoystick joystick) const;

private:
	class Xbox360ControllerInputImpl;
	Xbox360ControllerInputImpl* m_pImpl;
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
};
