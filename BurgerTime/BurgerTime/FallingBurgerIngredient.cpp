//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "FallingBurgerIngredient.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "BurgerTimeGlobal.h"
using namespace dae;
using namespace SimonGlobalEnums;
using namespace SimonGlobalConstants;
#include "Scene.h"
#include "IngredientPart.h"
#include "ComponentSpriteRenderer.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t FallingBurgerIngredient::GetTypeHash()
{
	return typeid(*this).hash_code();
}

FallingBurgerIngredient::FallingBurgerIngredient(dae::GameObject* pParent, const CollisionBox& collisionBox, IngredientType type, dae::Scene& scene)
	: ComponentBase(pParent)
	, m_CollisionBox{ collisionBox }
	, m_IngredientType{ type }
	, m_PartPtrs{}
	, m_NrDroppedParts{0}
	, m_FALLING_SPEED{CELL_WIDTH*2.5f}
	, m_IsFalling{false}
{
	CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::fallingBurgerIngredient), this);
	InitializeParts(scene);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void FallingBurgerIngredient::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}

void FallingBurgerIngredient::Update(float deltaTime)
{
	Vector2<float> collisionBoxWidthHeight{m_CollisionBox.GetWidthHeight()};
	Vector2<float> currentWorldPos{m_pParentGameObject->GetPosition()};
	m_CollisionBox = CollisionBox({ currentWorldPos.x - (collisionBoxWidthHeight.x/3), currentWorldPos.y}, collisionBoxWidthHeight.x, collisionBoxWidthHeight.y);

	if (m_IsFalling)
	{
		m_pParentGameObject->Move(Vector2<float>{ 0, m_FALLING_SPEED * deltaTime });
		for (IngredientPart* pPart : m_PartPtrs)
		{
			pPart->GetParent()->Move(Vector2<float>{ 0, m_FALLING_SPEED* deltaTime });
		}
	}
}

void FallingBurgerIngredient::InitializeParts(dae::Scene& scene)
{
	for (int partIdx{ 0 }; partIdx < 3; ++partIdx)
	{
		auto pPart{ std::make_shared<GameObject>() };
		pPart->SetPosition(Vector2<float>{ (partIdx - 1) * (m_CollisionBox.GetWidthHeight().x/3), 0 });
		m_pParentGameObject->AddChild(pPart.get());
		CollisionBox partCollisionBox{
			{},
			m_CollisionBox.GetWidthHeight().x/3,
			m_CollisionBox.GetWidthHeight().y
		};
		auto pIngredientPart{ new IngredientPart(pPart.get(),partCollisionBox) };
		pIngredientPart->AddObserver(this);
		pPart->AddComponent(pIngredientPart);
		Vector2<int> partSpriteOffset{ 112 + 8 * (2-partIdx), 48};
		switch (m_IngredientType)
		{
		case IngredientType::bunTop:
			break;
		case IngredientType::bunBottom:
			partSpriteOffset.y += 16;
			break;
		case IngredientType::cheese:
			partSpriteOffset.y += 16 * 2;
			break;
		case IngredientType::patty:
			partSpriteOffset.y += 16 * 3;
			break;
		case IngredientType::tomato:
			partSpriteOffset.y += 16 * 4;
			break;
		default:
			std::cout << "FallingBurgerIngredient::InitializeParts IngredientType not recognized: " << static_cast<int>(m_IngredientType) << std::endl;
			break;
		}
		ComponentSpriteRenderer* pPartSpriteRenderer{ new ComponentSpriteRenderer(
			pPart.get(),
			partCollisionBox.GetWidthHeight(),
			false,
			{}
			) };
		pPartSpriteRenderer->SetOffsetPixelsToNonAnimatedSprite(partSpriteOffset);
		pPartSpriteRenderer->SetSpritesheet(SPRITESHEET_PATH);
		pPart->AddComponent(pPartSpriteRenderer);

		scene.Add(pPart);
	}
}

void FallingBurgerIngredient::OnNotify(Event event, void*)
{
	if (event != Event::ingredientPartDropped) return;

	++m_NrDroppedParts;

	if (m_NrDroppedParts >= 3)
	{
		m_IsFalling = true;
		for (IngredientPart* pPart : m_PartPtrs)
		{
			pPart->SetIsDroppedFalse();
		}
	}
}