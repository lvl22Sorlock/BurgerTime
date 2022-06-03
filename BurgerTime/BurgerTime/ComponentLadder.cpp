//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "ComponentLadder.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "DebugManager.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
ComponentLadder::ComponentLadder(dae::GameObject* pParent, const CollisionBox& collisionBox, bool canClimbUp, bool canClimbDown)
	:ComponentBase(pParent)
	,m_CollisionBox{collisionBox}
	,m_CanClimbUp{canClimbUp}
	,m_CanClimbDown{canClimbDown}
{
	//CollisionManager::GetInstance().AddLadder(this);
}

ComponentLadder::~ComponentLadder()
{
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void ComponentLadder::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_pParentGameObject->GetPosition(), m_CollisionBox.GetWidthHeight());
}

const CollisionBox& ComponentLadder::GetCollisionBox() const
{
	return m_CollisionBox;
}

//float ComponentLadder::GetXPos() const
//{
//	return m_pParentGameObject->GetPosition().x;
//}

Vector2<float> ComponentLadder::GetPos() const
{
	return m_pParentGameObject->GetPosition();
}

bool ComponentLadder::GetGoesUp() const
{
	return m_CanClimbUp;
}

bool ComponentLadder::GetGoesDown() const
{
	return m_CanClimbDown;
}
