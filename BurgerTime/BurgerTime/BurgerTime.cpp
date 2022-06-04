#define _CRT_SECURE_NO_WARNINGS

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerTimePCH.h"

#include "Minigin.h"

#include <functional>

void LoadGame(void);

int main(int, char* [])
{
	dae::Minigin engine;
	std::function<void(void)> loadGameFunc{LoadGame};
	engine.SetLoadGameCallbackFunction(&loadGameFunc);
	engine.Run();

	return 0;
}

#pragma region OtherEngineIncludes
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "DebugManager.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#pragma endregion

#pragma region ComponentIncludes
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
#include "FPSCounterBehaviour.h"
#include "Level.h"
#pragma endregion

#include "BurgerTimeGlobal.h"
using namespace SimonGlobalConstants;

void LoadGame(void)
{
#ifdef _DEBUG
	DebugManager::GetInstance().SetIsDebugRendering(true);
#else
	DebugManager::GetInstance().SetIsDebugRendering(false);
#endif // _DEBUG


	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

#pragma region DefaultObjects
	auto go{ std::make_shared<GameObject>() };
	go->AddComponent(new ComponentTexture(go.get()));
	ComponentTexture* pTexture = dynamic_cast<ComponentTexture*>(go->GetComponentPtr(typeid(ComponentTexture)));
	if (pTexture) pTexture->SetTexture("background.jpg");
	pTexture = nullptr;
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->AddComponent(new ComponentTexture(go.get()));
	pTexture = dynamic_cast<ComponentTexture*>(go->GetComponentPtr(typeid(ComponentTexture)));
	if (pTexture) pTexture->SetTexture("logo.png");
	pTexture = nullptr;
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<GameObject>();
	to->AddComponent(new ComponentText(to.get(), "Programming 4 Assignment", font));
	to->SetPosition(80, 20);
	scene.Add(to);
#pragma endregion

#pragma region FPSCounter
	auto fpsCounter{ std::make_shared<GameObject>() };
	fpsCounter->AddComponent( new ComponentText(fpsCounter.get(), "FPSCOUNTER", font));
	fpsCounter->AddComponent(new ComponentTimer(fpsCounter.get()));
	fpsCounter->AddComponent(new FPSCounterBehaviour(fpsCounter.get()));
	scene.Add(fpsCounter);
#pragma endregion
	
#pragma region Level
	auto pLevel{ std::make_shared<GameObject>() };
	scene.Add(pLevel);
	auto pLevelComponent{ pLevel->GetAddComponent<Level>(new Level(pLevel.get())) };
	//pLevelComponent->MakeLadder({ 5, 12 }, 0, scene);
	//pLevelComponent->MakeLadder({ 0, 1 }, 0, scene);
	//pLevelComponent->MakePlatform({ 2,0 }, 8, scene);
	//pLevelComponent->MakePlatform({ 0,2 }, 5, scene);

	pLevelComponent->MakePlatform({ 0,2 }, 18, scene);
	pLevelComponent->MakeLadder({ 0, 4 }, 2, scene);
	pLevelComponent->MakeLadder({ 4, 4 }, 2, scene);
	pLevelComponent->MakeLadder({ 6, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 9, 5 }, 2, scene);
	pLevelComponent->MakeLadder({ 13, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 15, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 18, 7 }, 2, scene);
	pLevelComponent->MakePlatform({ 13,8 }, 18, scene);
	pLevelComponent->MakeLadder({ 18, 13 }, 11, scene);
	pLevelComponent->MakePlatform({ 13,11 }, 18, scene);

	pLevelComponent->MakePlatform({ 6,9 }, 13, scene);

	pLevelComponent->MakePlatform({ 0,5 }, 4, scene);
	pLevelComponent->MakeLadder({ 2, 10 }, 5, scene);
	pLevelComponent->MakeLadder({ 0, 13 }, 7, scene);
	pLevelComponent->MakePlatform({ 5,6 }, 9, scene);

	pLevelComponent->MakePlatform({ 0,7 }, 4, scene);
	pLevelComponent->MakePlatform({ 0,11 }, 12, scene);
	pLevelComponent->MakePlatform({ 0,14 }, 18, scene);

#pragma endregion
	
#pragma region PeterPepper
	const auto pPeterPepper{ std::make_shared<GameObject>() };
	pPeterPepper->SetPosition(SimonGlobalFunctions::GetPosFromIdx({ 0,2 }));

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
#pragma endregion

	
	#pragma region DisplayAssignment
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

#pragma region Sound
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack", "/Data/Sounds/Soundtrack.ogg");
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack2", "/Data/Sounds/Soundtrack.mp3");
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack3", "/Data/Sounds/Soundtrack.flac");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack2");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack3");
#pragma endregion

}