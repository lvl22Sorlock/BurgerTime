#define _CRT_SECURE_NO_WARNINGS

#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"

#include "Subject.h"
#include "Observer.h"
#include "Command.hpp"
#include "CollisionManager.h"
#include "SDL_mixer.h"
#include "ServiceLocator.h"
#include "SoundSystemSDL.h"

#pragma region COMPONENTS
#include "ComponentBase.h"
#include "ComponentText.h"
#include "ComponentTimer.h"
#include "FPSCounterBehaviour.h"
#include "ComponentTexture.h"
#include "ComponentHealth.h"
#include "ComponentHealthDisplay.h"
#include "ComponentScoreManager.h"
#include "ComponentScoreDisplay.h"
#include "ComponentCharacterController.h"
#include "ComponentSpriteRenderer.h"
#include "ComponentLadder.h"
#include "ComponentFollowOther.h"
#pragma endregion



// Additional includes
#include <chrono>
//

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO & SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	// SDL_Init(SDL_INIT_AUDIO) ??

	#pragma region SDL_Mixer
	// load support for the OGG and MOD sample/music formats
	int flags = MIX_INIT_OGG | MIX_INIT_MOD;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags) {
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 1024) == -1)
	{
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
	}
	#pragma endregion

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	m_pLoadGameCallbackFunction = nullptr;
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	if (m_pLoadGameCallbackFunction != nullptr)
		(*m_pLoadGameCallbackFunction)();
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;

	#pragma region SDL_Mixer
	//Mix_CloseAudio();
	//// force a quit
	//while (Mix_Init(0))
	//	Mix_Quit();
	#pragma endregion

	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	auto& input = InputManager::GetInstance();
	auto sound = SoundSystemSDL();
	ServiceLocator::GetInstance().RegisterSoundSystem(&sound);
	ServiceLocator::GetInstance().GetSoundSystem().LoadSound("Default", "../Data/MP3/01 BGM #01.mp3");
	sound.PlaySound(sound.GetSoundId("Default"));

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();


		// todo: this update loop could use some work.

		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		const float fixedTimeStep = 0.02f;

		bool doContinue = true;
		while (doContinue)
		{
			// Quit if necessary
			doContinue = input.ProcessInput();		

			// Input
			input.HandleInput();

			// Updates
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			// FixedUpdate (with fixed timestep)
			while (lag >= fixedTimeStep)
			{
				sceneManager.FixedUpdate(fixedTimeStep);
				lag -= fixedTimeStep;
			}
			// Update
			sceneManager.Update(deltaTime);			
			// LateUpdate
			sceneManager.LateUpdate(deltaTime);

			// Render
			renderer.Render();

			// Process Sounds
			sound.Update();
		}
	}

	Cleanup();
}

void Minigin::SetLoadGameCallbackFunction(std::function<void(void)>* pLoadGameCallbackFunction)
{
	m_pLoadGameCallbackFunction = pLoadGameCallbackFunction;
}
