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
#include <memory>
#include "Scene.h"
#include "ComponentLadder.h"
#include "ComponentSpriteRenderer.h"
#include "Platform.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t Level::GetTypeHash(){
	return typeid(*this).hash_code();
}

Level::Level(dae::GameObject* pParent)
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
	for (int idxX{ 0 }; idxX <= NR_PLATFORMS; ++idxX)
	{
		auto pPlatform{ std::make_shared<GameObject>() };
		m_pParentGameObject->AddChild(pPlatform.get());
		pPlatform->SetPosition(GetPosFromIdx({ idxX + leftIdcx.x, leftIdcx.y }));
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
	pPlatformLeftOnly->SetPosition(GetPosFromIdx({ leftIdcx.x-1, leftIdcx.y }));
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
	pPlatformRightOnly->SetPosition(GetPosFromIdx({ rightIdx + 1, leftIdcx.y }));
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