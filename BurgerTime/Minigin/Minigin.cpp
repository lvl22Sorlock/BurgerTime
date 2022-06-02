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
#include "GlobalVariables.h"
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
using namespace SimonGlobalVariables;

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
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

#pragma region DefaultObjects
	auto go{ std::make_shared<GameObject>() };
	go->AddComponent(/*"Texture",*/ new ComponentTexture(go.get()));
	ComponentTexture* pTexture = dynamic_cast<ComponentTexture*>(go->GetComponentPtr(typeid(ComponentTexture)/*"Texture"*/));
	if (pTexture) pTexture->SetTexture("background.jpg");
	pTexture = nullptr;
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->AddComponent(/*"Texture",*/ new ComponentTexture(go.get()));
	pTexture = dynamic_cast<ComponentTexture*>(go->GetComponentPtr(typeid(ComponentTexture)/*"Texture"*/));
	if (pTexture) pTexture->SetTexture("logo.png");
	pTexture = nullptr;
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<GameObject>();
	to->AddComponent(/*"Text", */new ComponentText(to.get(), "Programming 4 Assignment", font));
	to->SetPosition(80, 20);
	//ComponentText* pText = dynamic_cast<ComponentText*>(to->GetComponentPtr(typeid(ComponentText)/*"Text"*/));
	//pText->SetPosition(80, 20);
	scene.Add(to);
#pragma endregion

#pragma region FPSCounter
	auto fpsCounter{ std::make_shared<GameObject>() };
	fpsCounter->AddComponent(/*"Text",*/ new ComponentText(fpsCounter.get(), "FPSCOUNTER", font));
	fpsCounter->AddComponent(/*"Timer",*/ new ComponentTimer(fpsCounter.get()));
	fpsCounter->AddComponent(/*"FPSCounter",*/ new FPSCounterBehaviour(fpsCounter.get()));
	//const auto pCharacterControllerFPS{ fpsCounter->GetAddComponent<ComponentCharacterController>(new ComponentCharacterController(fpsCounter.get())) };
	//pCharacterControllerFPS->InitializeMovementInput(InputManager::GetInstance(), 'a', 'd', 'w', 's');
	scene.Add(fpsCounter);
#pragma endregion

#pragma region Ladders
	// a ladder
	//====	Top Ladder	====//
	auto pTestLadder{ std::make_shared<GameObject>() };
	pTestLadder->SetPosition(0, 0);
	scene.Add(pTestLadder);
	pTestLadder->AddComponent(new ComponentLadder(pTestLadder.get(), CollisionBox({ 0, 0 }, CELL_WIDTH, CELL_WIDTH), false, true));

	//====	Middle Ladders	====//
	auto pTestLadder2{ std::make_shared<GameObject>() };
	pTestLadder2->SetPosition(0, CELL_WIDTH);
	scene.Add(pTestLadder2);
	auto pComponentLadderSpriteRenderer2{ new ComponentSpriteRenderer(pTestLadder2.get(), {CELL_WIDTH,CELL_WIDTH}, false) };
	pComponentLadderSpriteRenderer2->SetSpritesheet("ladder.png");
	pTestLadder2->AddComponent(pComponentLadderSpriteRenderer2);
	pTestLadder2->AddComponent(new ComponentLadder(pTestLadder2.get(), CollisionBox({ 0, CELL_WIDTH }, CELL_WIDTH, CELL_WIDTH )));

	//====	Bottom Ladder	====//
	auto pTestLadder3{ std::make_shared<GameObject>() };
	pTestLadder3->SetPosition(0, 2* CELL_WIDTH);
	scene.Add(pTestLadder3);
	auto pComponentLadderSpriteRenderer3{ new ComponentSpriteRenderer(pTestLadder3.get(), {CELL_WIDTH,CELL_WIDTH}, false) };
	pComponentLadderSpriteRenderer3->SetSpritesheet("ladder.png");
	pTestLadder3->AddComponent(pComponentLadderSpriteRenderer3);
	pTestLadder3->AddComponent(new ComponentLadder(pTestLadder3.get(), CollisionBox({ 0, 2* CELL_WIDTH }, CELL_WIDTH, CELL_WIDTH), true, false));
	//// Ladders Debug Visualisation
	/*const auto pLadder1Collider{ std::make_shared<GameObject>() };
	const auto pLadder1ColliderTexture{ new ComponentTexture(pLadder1Collider.get(), Vector2<float>(50,50)) };
	pLadder1ColliderTexture->SetTexture("DebugGreenCollider3.png");
	pLadder1Collider->AddComponent(pLadder1ColliderTexture);
	scene.Add(pLadder1Collider);
	const auto pLadder2Collider{ std::make_shared<GameObject>() };
	pLadder2Collider->SetPosition({ 0, 50 });
	const auto pLadder2ColliderTexture{ new ComponentTexture(pLadder2Collider.get(), Vector2<float>(50,50)) };
	pLadder2ColliderTexture->SetTexture("DebugGreenCollider3.png");
	pLadder2Collider->AddComponent(pLadder2ColliderTexture);
	scene.Add(pLadder2Collider);
	const auto pLadder3Collider{ std::make_shared<GameObject>() };
	pLadder3Collider->SetPosition({ 0, 100 });
	const auto pLadder3ColliderTexture{ new ComponentTexture(pLadder3Collider.get(), Vector2<float>(50,50)) };
	pLadder3ColliderTexture->SetTexture("DebugGreenCollider3.png");
	pLadder3Collider->AddComponent(pLadder3ColliderTexture);
	scene.Add(pLadder3Collider);*/
	////
#pragma endregion

#pragma region PeterPepper
	// Peter Pepper, currently Peter Pepper is just a HealthDisplay object
	const auto pPeterPepper{ std::make_shared<GameObject>() };

	const auto pComponentHealth{ new ComponentHealth(pPeterPepper.get(), 3) };
	pPeterPepper->AddComponent(pComponentHealth);

	const auto pComponentScoreManager{ new ComponentScoreManager(pPeterPepper.get()) };
	pPeterPepper->AddComponent(pComponentScoreManager);
	
	const auto pCharacterController{ new ComponentCharacterController(pPeterPepper.get(), {CELL_WIDTH, CELL_WIDTH})};
	pPeterPepper->AddComponent(pCharacterController);
	pCharacterController->InitializeMovementInput(InputManager::GetInstance(), 'a', 'd', 'w', 's');

	const auto pSpriteRenderer{ new ComponentSpriteRenderer(pPeterPepper.get(), {CELL_WIDTH,CELL_WIDTH})};
	pPeterPepper->AddComponent(pSpriteRenderer);

	pSpriteRenderer->AddAnimation(L"WalkHorizontal", ComponentSpriteRenderer::AnimInfo(Vector2<int>(48, 0), 1, 3));
	pSpriteRenderer->TrySetAnimation(L"WalkHorizontal");
	pSpriteRenderer->SetSpritesheet("BurgerTimeSprites.png");

	pCharacterController->SetIsMovingAnim(pSpriteRenderer->GetIsActivePtr());
	pCharacterController->SetIsMovingRightAnim(pSpriteRenderer->GetIsMirroredPtr());

	scene.Add(pPeterPepper);

	// Peter Pepper debug collider visualisation
	/*const auto pPeterCollider{ std::make_shared<GameObject>() };
	const auto pPeterColliderTexture{ new ComponentTexture(pPeterCollider.get(), Vector2<float>(50,50)) };
	pPeterColliderTexture->SetTexture("DebugGreenCollider3.png");
	pPeterCollider->AddComponent(pPeterColliderTexture);
	pPeterCollider->AddComponent(new ComponentFollowOther(pPeterCollider.get(), pPeterPepper.get()));
	scene.Add(pPeterCollider);*/
#pragma endregion

#pragma region DisplayAssignment
	//
	const auto pHealthDisplay{ std::make_shared<GameObject>() };
	pHealthDisplay->AddComponent(new ComponentText(pHealthDisplay.get(), "HealthDisplayText", font));
	const auto pComponentHealthDisplay{new ComponentHealthDisplay(pHealthDisplay.get(), pPeterPepper.get())};
	pHealthDisplay->AddComponent(pComponentHealthDisplay);
	pHealthDisplay->SetPosition(0, 100);
	pComponentHealth->AddObserver(pComponentHealthDisplay);
	scene.Add(pHealthDisplay);

	const auto pScoreDisplay{ std::make_shared<GameObject>() };
	pScoreDisplay->AddComponent(new ComponentText(pScoreDisplay.get(), "ScoreDisplayText", font));
	const auto pComponentScoreDisplay{new ComponentScoreDisplay(pScoreDisplay.get(), pPeterPepper.get())};
	pScoreDisplay->AddComponent(pComponentScoreDisplay);
	pScoreDisplay->SetPosition(0, 130);
	pComponentScoreManager->AddObserver(pComponentScoreDisplay);
	scene.Add(pScoreDisplay);
#pragma endregion

#pragma region InputAssignment
	DieCommand* pDieCommand = new DieCommand(pPeterPepper.get());
	GainScoreCommand* pGainScoreCommand = new GainScoreCommand(pPeterPepper.get());
	auto& input = InputManager::GetInstance();

	input.AddCommandToCommandList(pDieCommand);
	input.AddCommandToCommandList(pGainScoreCommand);
	input.AddCommandToButton(dae::ControllerButton::ButtonA, pDieCommand);
	input.AddCommandToButton(dae::ControllerButton::ButtonX, pGainScoreCommand);



	pComponentScoreManager->IncreaseScore(500);
	pComponentScoreManager->IncreaseScore(2000);
	pComponentHealth->DealDamage(1);
	pComponentHealth->DealDamage(1);
	pComponentHealth->DealDamage(1);
#pragma endregion

#pragma region Sound

#pragma endregion

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
