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

#pragma region OtherEngineIncludes
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "DebugManager.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "StructsEnums.h"
using namespace inputEnums;
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
#include "EnemyController.h"
#include "FallingBurgerIngredient.h"
#include "MenuManager.h"
#pragma endregion

#include "BurgerTimeGlobal.h"
using namespace SimonGlobalConstants;
using namespace SimonGlobalEnums;
using namespace SimonGlobalFunctions;

#include <fstream>

void LoadGame(void);

static dae::Minigin* gEnginePtr{ nullptr };


int main(int, char* [])
{
	dae::Minigin engine{};
	gEnginePtr = &engine;
	std::function<void(void)> loadGameFunc{LoadGame};
	engine.SetLoadGameCallbackFunction(&loadGameFunc);
	engine.Run();

	return 0;
}



void LoadGame(void)
{
//#ifdef _DEBUG
//	DebugManager::GetInstance().SetIsDebugRendering(true);
//#else
//	DebugManager::GetInstance().SetIsDebugRendering(false);
//#endif // _DEBUG


	auto& scene = SceneManager::GetInstance().CreateScene("BurgerTime");

#pragma region DefaultObjects
	const auto backgroundObject{ std::make_shared<GameObject>() };	
	ComponentTexture* pTexture 
		= backgroundObject->GetAddComponent<ComponentTexture>(new ComponentTexture(backgroundObject.get()));
	if (pTexture) pTexture->SetTexture("background.jpg");
	pTexture = nullptr;
	scene.Add(backgroundObject);

	const auto logoObject{ std::make_shared<GameObject>() };	
	pTexture 
		= logoObject->GetAddComponent<ComponentTexture>(new ComponentTexture(logoObject.get(), {208*0.85f, 65*0.85f}));
	if (pTexture) pTexture->SetTexture("logo.png");
	pTexture = nullptr;
	logoObject->SetPosition(242.5f, 190); //(216, 210);
	scene.Add(logoObject);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
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
	//std::ofstream outputLevelFileStream{pLevelComponent->GetLevelOutputFileStream("Level01")};
	//if (outputLevelFileStream.is_open())
	//{
	//	pLevelComponent->MakePlatformFile({ 0,2 }, 18, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 0, 4 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 4, 4 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 6, 13 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 9, 5 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 13, 13 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 15, 13 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 18, 7 }, 2, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 13,8 }, 18, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 18, 13 }, 11, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 13,11 }, 18, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 6,9 }, 13, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 0,5 }, 4, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 2, 10 }, 5, outputLevelFileStream);
	//	pLevelComponent->MakeLadderFile({ 0, 13 }, 7, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 5,6 }, 9, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 0,7 }, 4, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 0,11 }, 12, outputLevelFileStream);
	//	pLevelComponent->MakePlatformFile({ 0,14 }, 18, outputLevelFileStream);
	//}
	//pLevelComponent->ParseFile("./../Data/Levels/Level01.txt", scene);

	pLevelComponent->MakePlatform({ 0,2 }, 18, scene);
	pLevelComponent->MakePlatform({ 13,8 }, 18, scene);
	pLevelComponent->MakePlatform({ 13,11 }, 18, scene);
	pLevelComponent->MakePlatform({ 6,9 }, 13, scene);
	pLevelComponent->MakePlatform({ 0,5 }, 4, scene);
	pLevelComponent->MakePlatform({ 5,6 }, 9, scene);
	pLevelComponent->MakePlatform({ 0,7 }, 4, scene);
	pLevelComponent->MakePlatform({ 0,11 }, 12, scene);
	pLevelComponent->MakePlatform({ 0,14 }, 18, scene);
	pLevelComponent->MakeLadder({ 0, 4 }, 2, scene);
	pLevelComponent->MakeLadder({ 4, 4 }, 2, scene);
	pLevelComponent->MakeLadder({ 6, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 9, 5 }, 2, scene);
	pLevelComponent->MakeLadder({ 13, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 15, 13 }, 2, scene);
	pLevelComponent->MakeLadder({ 18, 7 }, 2, scene);
	pLevelComponent->MakeLadder({ 18, 13 }, 11, scene);
	pLevelComponent->MakeLadder({ 2, 10 }, 5, scene);
	pLevelComponent->MakeLadder({ 0, 13 }, 7, scene);


	pLevelComponent->MakeIngredient({ 2,2 }, IngredientType::bunTop, scene);
	pLevelComponent->MakeIngredient({ 2,5 }, IngredientType::patty, scene);
	pLevelComponent->MakeIngredient({ 2,7 }, IngredientType::cheese, scene);
	pLevelComponent->MakeIngredient({ 2,11 }, IngredientType::tomato, scene);
	pLevelComponent->MakeIngredient({ 2,14 }, IngredientType::bunBottom, scene);

	pLevelComponent->MakeWaitingPlate({ 2,16 }, scene);
#pragma endregion
	

#pragma region PeterPepper
	
	const auto pPeterPepper{ std::make_shared<GameObject>() };
	pPeterPepper->SetPosition(GetPosFromIdx({ 0,2 }));

	const auto pCharacterController{ new ComponentCharacterController(pPeterPepper.get(), {CELL_WIDTH, CELL_WIDTH}, true) };
	pPeterPepper->AddComponent(pCharacterController);
	pCharacterController->InitializeMovementInput(InputManager::GetInstance(), 1);

	const auto pSpriteRenderer{ new ComponentSpriteRenderer(pPeterPepper.get(), {CELL_WIDTH,CELL_WIDTH}) };
	pPeterPepper->AddComponent(pSpriteRenderer);
	pCharacterController->SetSpriteRenderer(pSpriteRenderer);

	pCharacterController->SetIsMovingAnim(pSpriteRenderer->GetIsActivePtr());
	pCharacterController->SetIsMovingRightAnim(pSpriteRenderer->GetIsMirroredPtr());

	scene.Add(pPeterPepper);
	
	
	const auto pMrsSalt{ std::make_shared<GameObject>() };
	pMrsSalt->SetPosition(GetPosFromIdx({ 8,2 }));

	const auto pMrsSaltCharacterController{ new ComponentCharacterController(pMrsSalt.get(), {CELL_WIDTH, CELL_WIDTH}, true) };
	pMrsSalt->AddComponent(pMrsSaltCharacterController);
	pMrsSaltCharacterController->InitializeMovementInput(InputManager::GetInstance(), 1);

	const auto pMrsSaltSpriteRenderer{ new ComponentSpriteRenderer(pMrsSalt.get(), {CELL_WIDTH,CELL_WIDTH}) };
	pMrsSalt->AddComponent(pMrsSaltSpriteRenderer);
	pMrsSaltCharacterController->SetSpriteRenderer(pMrsSaltSpriteRenderer);

	pMrsSaltCharacterController->SetIsMovingAnim(pMrsSaltSpriteRenderer->GetIsActivePtr());
	pMrsSaltCharacterController->SetIsMovingRightAnim(pMrsSaltSpriteRenderer->GetIsMirroredPtr());

	scene.Add(pMrsSalt);
	
#pragma endregion


#pragma region Enemies
	std::vector<const ComponentCharacterController*> m_PlayerPtrs{ pCharacterController, pMrsSaltCharacterController };
	auto pMrEnemy{ std::make_shared<GameObject>() };
	pMrEnemy->SetPosition(SimonGlobalFunctions::GetPosFromIdx({ 18, 12 }));

	auto pEnemyCharacterController{ new ComponentCharacterController(pMrEnemy.get(), {CELL_WIDTH, CELL_WIDTH}) };
	pMrEnemy->AddComponent(pEnemyCharacterController);
	auto pEnemySpriteRenderer{new ComponentSpriteRenderer(pMrEnemy.get(), {CELL_WIDTH,CELL_WIDTH})};
	pMrEnemy->AddComponent(pEnemySpriteRenderer);
	auto pEnemyEnemyController{ new EnemyController(pMrEnemy.get(), SimonGlobalEnums::CharacterType::mrHotDog) };
	pMrEnemy->AddComponent(pEnemyEnemyController);
	pEnemyEnemyController->SetPlayerPtrs(m_PlayerPtrs);
	pEnemyEnemyController->SetCharacterController(pEnemyCharacterController);
	pEnemyCharacterController->SetSpriteRenderer(pEnemySpriteRenderer);
	pEnemyCharacterController->SetIsMovingAnim(pEnemySpriteRenderer->GetIsActivePtr());
	pEnemyCharacterController->SetIsMovingRightAnim(pEnemySpriteRenderer->GetIsMirroredPtr());
	scene.Add(pMrEnemy);

	pMrEnemy =  std::make_shared<GameObject>() ;
	pMrEnemy->SetPosition(SimonGlobalFunctions::GetPosFromIdx({ 9, 14 }));
	pEnemyCharacterController =  new ComponentCharacterController(pMrEnemy.get(), {CELL_WIDTH, CELL_WIDTH}) ;
	pMrEnemy->AddComponent(pEnemyCharacterController);
	pEnemySpriteRenderer =  new ComponentSpriteRenderer(pMrEnemy.get(), {CELL_WIDTH,CELL_WIDTH}) ;
	pMrEnemy->AddComponent(pEnemySpriteRenderer);
	pEnemyEnemyController =  new EnemyController(pMrEnemy.get(), SimonGlobalEnums::CharacterType::mrPickle, true) ;
	pMrEnemy->AddComponent(pEnemyEnemyController);
	pEnemyEnemyController->SetPlayerPtrs(m_PlayerPtrs);
	pEnemyEnemyController->SetCharacterController(pEnemyCharacterController);
	pEnemyCharacterController->SetSpriteRenderer(pEnemySpriteRenderer);
	pEnemyCharacterController->SetIsMovingAnim(pEnemySpriteRenderer->GetIsActivePtr());
	pEnemyCharacterController->SetIsMovingRightAnim(pEnemySpriteRenderer->GetIsMirroredPtr());
	scene.Add(pMrEnemy);

	pMrEnemy = std::make_shared<GameObject>();
	pMrEnemy->SetPosition(SimonGlobalFunctions::GetPosFromIdx({ 0, 12 }));
	pEnemyCharacterController = new ComponentCharacterController(pMrEnemy.get(), { CELL_WIDTH, CELL_WIDTH });
	pMrEnemy->AddComponent(pEnemyCharacterController);
	pEnemySpriteRenderer = new ComponentSpriteRenderer(pMrEnemy.get(), { CELL_WIDTH,CELL_WIDTH });
	pMrEnemy->AddComponent(pEnemySpriteRenderer);
	pEnemyEnemyController = new EnemyController(pMrEnemy.get(), SimonGlobalEnums::CharacterType::mrEgg);
	pMrEnemy->AddComponent(pEnemyEnemyController);
	pEnemyEnemyController->SetPlayerPtrs(m_PlayerPtrs);
	pEnemyEnemyController->SetCharacterController(pEnemyCharacterController);
	pEnemyCharacterController->SetSpriteRenderer(pEnemySpriteRenderer);
	pEnemyCharacterController->SetIsMovingAnim(pEnemySpriteRenderer->GetIsActivePtr());
	pEnemyCharacterController->SetIsMovingRightAnim(pEnemySpriteRenderer->GetIsMirroredPtr());
	scene.Add(pMrEnemy);
#pragma endregion


#pragma region Sound
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack", "./../Data/Sounds/Soundtrack.ogg");
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack2", "./../Data/Sounds/Soundtrack.mp3");
		ServiceLocator::GetInstance().GetSoundSystem()->AddSound("Soundtrack3", "./../Data/Sounds/Soundtrack.flac");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack2");
		ServiceLocator::GetInstance().GetSoundSystem()->PlaySound("Soundtrack3");
#pragma endregion

#pragma region Menus
		MenuManager::GetInstance().SetEngine(gEnginePtr);
		MenuManager::GetInstance().SetScene(&scene);
		MenuManager::GetInstance().GoToMainMenu();

		MenuManager::TogglePauseMenuCommand* pToggleMenuCommand{ new MenuManager::TogglePauseMenuCommand() };
		InputManager::GetInstance().AddCommandToCommandList(pToggleMenuCommand);
		InputManager::GetInstance().AddCommandToButton('P', InputType::IsPressed, pToggleMenuCommand);
		InputManager::GetInstance().AddCommandToButton(ControllerButton::options, InputType::IsPressed, pToggleMenuCommand, 1);
		InputManager::GetInstance().AddCommandToButton(ControllerButton::options, InputType::IsPressed, pToggleMenuCommand, 2);
#pragma endregion
}