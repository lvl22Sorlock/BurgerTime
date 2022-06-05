//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "IngredientPart.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "BurgerTimeGlobal.h"
using namespace SimonGlobalEnums;

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t IngredientPart::GetTypeHash()
{
	return typeid(*this).hash_code();
}

IngredientPart::IngredientPart(dae::GameObject* pParent, const CollisionBox& collisionBox)
	: ComponentBase(pParent)
	, m_CollisionBox{ collisionBox }
	, m_WasCollidingWithPlayer{false}
	, m_IsDropped{false}
	, m_DROPPED_POS_Y_OFFSET{collisionBox.GetWidthHeight().y/4.0f}
	, m_DROPPED_COOLDOWN{0.25f}
	, m_CurrentDroppedCooldown{0.0f}
	, m_IsOnPlate{false}
{
	Vector2<float> currentPos{ m_pParentGameObject->GetLocalPosition() };
	m_pParentGameObject->SetPosition(currentPos.x, currentPos.y);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void IngredientPart::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}

void IngredientPart::Update(float deltaTime)
{
	if (m_IsOnPlate) return;
	m_CurrentDroppedCooldown -= deltaTime;
	if (m_CurrentDroppedCooldown > 0) return;

	Vector2<float> collisionBoxWidthHeigth{ m_CollisionBox.GetWidthHeight() };
	m_CollisionBox = CollisionBox(m_pParentGameObject->GetPosition(), collisionBoxWidthHeigth.x, collisionBoxWidthHeigth.y);
	bool isCollidingWithPlayer{ CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::player), m_CollisionBox, nullptr) };
	if (!isCollidingWithPlayer
		&& 
		m_WasCollidingWithPlayer
		&&
		!m_IsDropped)
	{
		NotifyObservers(Observer::Event::ingredientPartDropped);
		Vector2<float> currentPos{ m_pParentGameObject->GetLocalPosition()};
		m_pParentGameObject->SetPosition(currentPos.x, currentPos.y + m_DROPPED_POS_Y_OFFSET);
		m_IsDropped = true;
		m_CurrentDroppedCooldown = m_DROPPED_COOLDOWN;
	}

	m_WasCollidingWithPlayer = isCollidingWithPlayer;
}

void IngredientPart::SetIsDroppedFalse()
{
	if (!m_IsDropped) return;
	m_IsDropped = false;
	Vector2<float> currentPos{ m_pParentGameObject->GetLocalPosition() };
	m_pParentGameObject->SetPosition(currentPos.x, currentPos.y - m_DROPPED_POS_Y_OFFSET);
	m_CurrentDroppedCooldown = m_DROPPED_COOLDOWN;
	m_WasCollidingWithPlayer = false;
}

void IngredientPart::SetIsOnPlate()
{

}