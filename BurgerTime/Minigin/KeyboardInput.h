#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	KeyboardInput Class
//-------------------------------------------------------------------------
class KeyboardInput final 
{
public:
	KeyboardInput();
	~KeyboardInput();

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	KeyboardInput(const KeyboardInput& other) = delete;
	KeyboardInput(KeyboardInput&& other) noexcept = delete;
	KeyboardInput& operator=(const KeyboardInput& other) = delete;
	KeyboardInput& operator=(KeyboardInput&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void Update();
	bool IsUp(char button) const;
	bool IsDown(char button) const;
	bool IsPressed(char button) const;
	bool IsReleased(char button) const;

private:
	class KeyboardInputImpl;
	KeyboardInputImpl* m_pImpl;
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
};
