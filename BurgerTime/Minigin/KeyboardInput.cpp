//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "KeyboardInput.h"

//-------------------------------------------------------------------------
//	Implementation Includes
//-------------------------------------------------------------------------
#include "../imgui-1.87/imgui_impl_sdl.h"
#include <unordered_map>

#pragma region Implementation
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	Implementation Class
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

class KeyboardInput::KeyboardInputImpl final
{
public:
	KeyboardInputImpl();
	~KeyboardInputImpl() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	KeyboardInputImpl(const KeyboardInputImpl& other) = delete;
	KeyboardInputImpl(KeyboardInputImpl&& other) noexcept = delete;
	KeyboardInputImpl& operator=(const KeyboardInputImpl& other) = delete;
	KeyboardInputImpl& operator=(KeyboardInputImpl&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void Update();
	bool IsUp(char button) const;
	bool IsDown(char button) const;
	bool IsPressed(char button) const;
	bool IsReleased(char button) const;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------
	char GetKeyChar(int scanCode) const;
	char GetCapitalChar(char possiblyCapitalChar) const;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
	std::unordered_map<char, bool> m_SDLReleasedKeys;
	std::unordered_map<char, bool> m_SDLPressedKeys;
	std::unordered_map<char, bool> m_SDLDownKeys;
};

KeyboardInput::KeyboardInputImpl::KeyboardInputImpl()
{
	m_SDLPressedKeys.reserve(50);
	m_SDLReleasedKeys.reserve(50);
}

void KeyboardInput::KeyboardInputImpl::Update()
{
	SDL_Event e;
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
		switch (e.type)
		{
		case SDL_QUIT:
			return;
			//break;
		case SDL_KEYDOWN:
			m_SDLPressedKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
			m_SDLDownKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
			break;
		case SDL_KEYUP:
			m_SDLReleasedKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = true;
			m_SDLDownKeys[GetKeyChar(static_cast<int>(e.key.keysym.scancode))] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		}

		//process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
}

char KeyboardInput::KeyboardInputImpl::GetKeyChar(int scanCode) const
{
	const int NR_A_SDL{ 4 };
	const int NR_A_CHAR{ static_cast<int>('A') };
	return static_cast<char>(scanCode + (NR_A_CHAR - NR_A_SDL));
}

char KeyboardInput::KeyboardInputImpl::GetCapitalChar(char possiblyCapitalChar) const
{
	possiblyCapitalChar = static_cast<char>(std::toupper(possiblyCapitalChar));

	return possiblyCapitalChar;
}

bool  KeyboardInput::KeyboardInputImpl::IsUp(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLDownKeys.find(capitalButton) };
	return (buttonIterator == m_SDLDownKeys.end()
		||
		(*buttonIterator).second == false);
}

bool  KeyboardInput::KeyboardInputImpl::IsDown(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLDownKeys.find(capitalButton) };
	if (buttonIterator == m_SDLDownKeys.end()) return false;
	return (*buttonIterator).second;
}

bool  KeyboardInput::KeyboardInputImpl::IsPressed(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLPressedKeys.find(capitalButton) };
	if (buttonIterator == m_SDLPressedKeys.end()) return false;
	return (*buttonIterator).second;
}

bool  KeyboardInput::KeyboardInputImpl::IsReleased(char button) const
{
	char capitalButton{ GetCapitalChar(button) };
	auto buttonIterator{ m_SDLReleasedKeys.find(capitalButton) };
	if (buttonIterator == m_SDLReleasedKeys.end()) return false;
	return (*buttonIterator).second;
}
#pragma endregion




//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	KeyboardInput Class
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
KeyboardInput::KeyboardInput()
	:m_pImpl{new KeyboardInputImpl()}
{

}

KeyboardInput::~KeyboardInput()
{
	delete m_pImpl;
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void KeyboardInput::Update() {
	m_pImpl->Update();
}

bool KeyboardInput::IsUp(char button) const{
	return m_pImpl->IsUp(button);
}

bool KeyboardInput::IsDown(char button) const{
	return m_pImpl->IsDown(button);
}

bool KeyboardInput::IsPressed(char button) const{
	return m_pImpl->IsPressed(button);
}

bool KeyboardInput::IsReleased(char button) const{
	return m_pImpl->IsReleased(button);
}