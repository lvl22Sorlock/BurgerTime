//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "Level.h"
#include "GameObject.h"
using namespace dae;

#include "BurgerTimeGlobal.h"
using namespace SimonGlobalFunctions;
using namespace SimonGlobalConstants;
using namespace SimonGlobalEnums;
#include <memory>
#include "Scene.h"
#include "ComponentLadder.h"
#include "ComponentSpriteRenderer.h"
#include "Platform.h"
#include <fstream>
#include <iostream>
#include "WaitingPlate.h"
#include "FallingBurgerIngredient.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t Level::GetTypeHash(){
	return typeid(*this).hash_code();
}

Level::Level(GameObject* pParent)
	:ComponentBase(pParent)
{}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void Level::MakeLadder(const Vector2<int>& topIdcs, int bottomIdx, Scene& scene)
{
	const int NR_LADDERS{ topIdcs.y - bottomIdx + 1 };

	//====	Bottom Ladder	====//
	auto pLadderUpwards{ std::make_shared<GameObject>()};
	m_pParentGameObject->AddChild(pLadderUpwards.get());
	scene.Add(pLadderUpwards);

	pLadderUpwards->SetPosition(GetPosFromIdx({ topIdcs.x, topIdcs.y + 1 }));
	auto pLadderUpwardsSpriteRenderer{ new ComponentSpriteRenderer(pLadderUpwards.get(), {CELL_WIDTH,CELL_WIDTH}, false) };
	pLadderUpwardsSpriteRenderer->SetSpritesheet("ladder.png");
	pLadderUpwards->AddComponent(pLadderUpwardsSpriteRenderer);
	CollisionBox ladderCollisionBox{GetPosFromIdx({ topIdcs.x, topIdcs.y + 1}), CELL_WIDTH/3, CELL_WIDTH};
	ladderCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
	ladderCollisionBox.rightTop.x += (CELL_WIDTH / 3);
	pLadderUpwards->AddComponent(
		new ComponentLadder(
			pLadderUpwards.get(),
			ladderCollisionBox,
			true,
			false)
	);

	//====	Top Ladder	====//
	auto pLadderDownwards{ std::make_shared<GameObject>() };
	m_pParentGameObject->AddChild(pLadderDownwards.get());
	pLadderDownwards->SetPosition(GetPosFromIdx({ topIdcs.x, bottomIdx }));
	scene.Add(pLadderDownwards);
	ladderCollisionBox = CollisionBox(GetPosFromIdx({ topIdcs.x, bottomIdx }), CELL_WIDTH / 3, CELL_WIDTH);
	ladderCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
	ladderCollisionBox.rightTop.x += (CELL_WIDTH / 3);
	pLadderDownwards->AddComponent(
		new ComponentLadder(
			pLadderDownwards.get(),
			ladderCollisionBox,
			false,
			true)
	);

	if (NR_LADDERS < 2) return;
	//====	Middle Ladders	====//
	for (int idxY{ 1 }; idxY < NR_LADDERS; ++idxY)
	{
		auto pLadder{ std::make_shared<GameObject>() };
		m_pParentGameObject->AddChild(pLadder.get());
		pLadder->SetPosition(GetPosFromIdx({ topIdcs.x, idxY + bottomIdx }));
		scene.Add(pLadder);
		auto pLadderSpriteRenderer{ new ComponentSpriteRenderer(pLadder.get(), {CELL_WIDTH,CELL_WIDTH}, false) };
		pLadderSpriteRenderer->SetSpritesheet("ladder.png");
		pLadder->AddComponent(pLadderSpriteRenderer);
		ladderCollisionBox = CollisionBox(GetPosFromIdx({ topIdcs.x, idxY + bottomIdx }), CELL_WIDTH / 3, CELL_WIDTH);
		ladderCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
		ladderCollisionBox.rightTop.x += (CELL_WIDTH / 3);
		pLadder->AddComponent(
			new ComponentLadder(
				pLadder.get(),
				ladderCollisionBox
			)
		);
	}
}

void Level::MakePlatform(const Vector2<int>& leftIdcx, int rightIdx, dae::Scene& scene)
{
	const int NR_PLATFORMS{ rightIdx - leftIdcx.x };
	const Vector2<float> PLATFORM_SPRITE_OFFSET{ 0,5.0f };
	for (int idxX{ 0 }; idxX <= NR_PLATFORMS; ++idxX)
	{
		auto pPlatform{ std::make_shared<GameObject>() };
		m_pParentGameObject->AddChild(pPlatform.get());
		pPlatform->SetPosition(GetPosFromIdx({ idxX + leftIdcx.x, leftIdcx.y }) + PLATFORM_SPRITE_OFFSET);
		scene.Add(pPlatform);
		auto pLadderSpriteRenderer{ new ComponentSpriteRenderer(pPlatform.get(), {CELL_WIDTH,CELL_WIDTH}, false) };
		pLadderSpriteRenderer->SetSpritesheet("platform.png");
		pPlatform->AddComponent(pLadderSpriteRenderer);
		CollisionBox platformCollisionBox = CollisionBox(GetPosFromIdx({ idxX + leftIdcx.x, leftIdcx.y }), CELL_WIDTH / 3, CELL_WIDTH / 3);
		platformCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
		platformCollisionBox.rightTop.x += (CELL_WIDTH / 3);
		platformCollisionBox.leftBottom.y += (CELL_WIDTH / 3);
		platformCollisionBox.rightTop.y += (CELL_WIDTH / 3);
		pPlatform->AddComponent(
			new Platform(
				pPlatform.get(),
				platformCollisionBox
			)
		);
	}

	// Invisible left-only platform on the right
	auto pPlatformLeftOnly{ std::make_shared<GameObject>() };
	m_pParentGameObject->AddChild(pPlatformLeftOnly.get());
	pPlatformLeftOnly->SetPosition(GetPosFromIdx({ leftIdcx.x-1, leftIdcx.y }) + PLATFORM_SPRITE_OFFSET);
	scene.Add(pPlatformLeftOnly);
	CollisionBox platformCollisionBox{ CollisionBox(GetPosFromIdx({leftIdcx.x - 1, leftIdcx.y }), CELL_WIDTH / 3, CELL_WIDTH / 3) };
	platformCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
	platformCollisionBox.rightTop.x += (CELL_WIDTH / 3);
	platformCollisionBox.leftBottom.y += (CELL_WIDTH / 3);
	platformCollisionBox.rightTop.y += (CELL_WIDTH / 3);
	pPlatformLeftOnly->AddComponent(
		new Platform(
			pPlatformLeftOnly.get(),
			platformCollisionBox,
			true,
			false
		)
	);
	// Invisible right-only platform on the left
	auto pPlatformRightOnly{ std::make_shared<GameObject>() };
	m_pParentGameObject->AddChild(pPlatformRightOnly.get());
	pPlatformRightOnly->SetPosition(GetPosFromIdx({ rightIdx + 1, leftIdcx.y }) + PLATFORM_SPRITE_OFFSET);
	scene.Add(pPlatformRightOnly);
	platformCollisionBox = CollisionBox(GetPosFromIdx({ rightIdx + 1, leftIdcx.y }), CELL_WIDTH / 3, CELL_WIDTH / 3);
	platformCollisionBox.leftBottom.x += (CELL_WIDTH / 3);
	platformCollisionBox.rightTop.x += (CELL_WIDTH / 3);
	platformCollisionBox.leftBottom.y += (CELL_WIDTH / 3);
	platformCollisionBox.rightTop.y += (CELL_WIDTH / 3);
	pPlatformRightOnly->AddComponent(
		new Platform(
			pPlatformRightOnly.get(),
			platformCollisionBox,
			true,
			true
		)
	);
}

void Level::MakeWaitingPlate(const Vector2<int>& leftIdcs, dae::Scene& scene)
{
	auto pWaitingPlateObject{ std::make_shared<GameObject>() };
	m_pParentGameObject->AddChild(pWaitingPlateObject.get());
	pWaitingPlateObject->SetPosition(GetPosFromIdx({ leftIdcs.x - 1, leftIdcs.y }));
	scene.Add(pWaitingPlateObject);
	auto pWaitingPlateSpriteRenderer{ new ComponentSpriteRenderer(pWaitingPlateObject.get(), {CELL_WIDTH * 3, CELL_WIDTH}, false) };
	pWaitingPlateSpriteRenderer->SetSpritesheet("waitingPlate.png");
	pWaitingPlateObject->AddComponent(pWaitingPlateSpriteRenderer);
	CollisionBox platformCollisionBox = CollisionBox(GetPosFromIdx({ leftIdcs.x - 1, leftIdcs.y }), CELL_WIDTH * 3, CELL_WIDTH / 3);
	platformCollisionBox.rightTop.x += (CELL_WIDTH / 3);
	platformCollisionBox.rightTop.y += 1.5f*(CELL_WIDTH / 3);
	platformCollisionBox.leftBottom.y += 1.5f*(CELL_WIDTH / 3);
	pWaitingPlateObject->AddComponent(
		new WaitingPlate(
			pWaitingPlateObject.get(),
			platformCollisionBox
		)
	);
}

void Level::MakeIngredient(const Vector2<int>& leftIdcs, IngredientType type, dae::Scene& scene)
{
	auto pIngredient{ std::make_shared<GameObject>() };
	m_pParentGameObject->AddChild(pIngredient.get());
	pIngredient->SetPosition(GetPosFromIdx(leftIdcs));
	auto pIngredientComponent{ new FallingBurgerIngredient(pIngredient.get(), {{0,0}, CELL_WIDTH * 3, CELL_WIDTH / 3 }, type, scene) };
	pIngredient->AddComponent(pIngredientComponent);
	scene.Add(pIngredient);
}

void Level::ParseFile(const std::string& filePath, dae::Scene& scene)
{
	std::ifstream ifs{};
	ifs.open(filePath);
	if (ifs.is_open())
	{
		while (ifs)
		{
			ParseFileBlock(ifs, scene);
		}
	}
	else
		std::cout << "Level failed to open file with path " << filePath << std::endl;
}

void Level::ParseFileBlock(std::ifstream& ifs, dae::Scene& scene)
{
	char byteArray1Block[4]{};
	ifs.read(&byteArray1Block[0], 4);
	BlockId blockId{ static_cast<BlockId>(GetIntFrom4Chars(&byteArray1Block[0]))};
	Vector2<int> topIdcs{};
	int bottomIdx{};
	Vector2<int> leftIdcs{};
	int rightIdx{};
	switch (blockId)
	{
	case BlockId::ladder:
		ifs.read(&byteArray1Block[0], 4);
		topIdcs.x = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		ifs.read(&byteArray1Block[0], 4);
		topIdcs.y = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		ifs.read(&byteArray1Block[0], 4);
		bottomIdx = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		MakeLadder(topIdcs, bottomIdx, scene);
		break;
	case BlockId::platform:
		ifs.read(&byteArray1Block[0], 4);
		leftIdcs.x = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		ifs.read(&byteArray1Block[0], 4);
		leftIdcs.y = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		ifs.read(&byteArray1Block[0], 4);
		rightIdx = static_cast<int>(GetIntFrom4Chars(&byteArray1Block[0]));
		MakePlatform(leftIdcs, rightIdx, scene);
		break;
	default:
		std::cout << "Error while parsing level file when attempting to read blockId" << std::endl;
		return;
	}
}

void Level::MakeLadderFile(const Vector2<int>& topIdcs, int bottomIdx, std::ofstream& ofs)
{
	std::vector<char> byteArray{};
	GetIntToBytes(static_cast<int32_t>(BlockId::ladder), byteArray);
	GetIntToBytes(static_cast<int32_t>(topIdcs.x), byteArray);
	GetIntToBytes(static_cast<int32_t>(topIdcs.y), byteArray);
	GetIntToBytes(static_cast<int32_t>(bottomIdx), byteArray);
	ofs.write(&byteArray[0], byteArray.size());
}

void Level::MakePlatformFile(const Vector2<int>& leftIdcx, int rightIdx, std::ofstream& ofs)
{
	std::vector<char> byteArray{};
	GetIntToBytes(static_cast<int32_t>(BlockId::platform), byteArray);
	GetIntToBytes(static_cast<int32_t>(leftIdcx.x), byteArray);
	GetIntToBytes(static_cast<int32_t>(leftIdcx.y), byteArray);
	GetIntToBytes(static_cast<int32_t>(rightIdx), byteArray);
	ofs.write(&byteArray[0], byteArray.size());
}

std::ofstream Level::GetLevelOutputFileStream(const std::string& levelName)
{
	std::ofstream outputFileStream{};
	outputFileStream.open("./../Data/Levels/" + levelName + ".txt", std::ofstream::out | std::ofstream::binary);
	return outputFileStream;
}

// https://stackoverflow.com/questions/5585532/c-int-to-byte-array
std::vector<char>& Level::GetIntToBytes(int32_t integer, std::vector<char>& byteArrayVector)
{
	byteArrayVector.emplace_back(static_cast<char>(integer & (0x000000ff)));
	byteArrayVector.emplace_back(static_cast<char>((integer & (0x000000ff << 8)) >> 8));
	byteArrayVector.emplace_back(static_cast<char>((integer & (0x000000ff << 16)) >> 16));
	byteArrayVector.emplace_back(static_cast<char>((integer & (0x000000ff << 24)) >> 24));
	return byteArrayVector;
}

int32_t Level::GetIntFrom4Chars(char* pFirstchar)
{
	int32_t output{};
	output = output | static_cast<int32_t>((pFirstchar[0]));
	output = output | static_cast<int32_t>((pFirstchar[1]) << 8);
	output = output | static_cast<int32_t>((pFirstchar[2]) << 16);
	output = output | static_cast<int32_t>((pFirstchar[3]) << 24);
	return output;
}