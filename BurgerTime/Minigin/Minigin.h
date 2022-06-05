#pragma once
struct SDL_Window;

#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		Minigin() : m_Window{ nullptr }, m_pLoadGameCallbackFunction{}, m_IsTimePaused{ false }{}

		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();

		void SetLoadGameCallbackFunction(std::function<void(void)>* pLoadGameCallbackFunction);
		void SetTimePaused(bool isPaused);
	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};

		std::function<void(void)>* m_pLoadGameCallbackFunction = nullptr;
		bool m_IsTimePaused;
	};
}