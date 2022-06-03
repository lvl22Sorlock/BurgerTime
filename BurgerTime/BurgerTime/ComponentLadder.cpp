//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "ComponentLadder.h"
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

std::size_t ComponentLadder::GetTypeHash()
{
	return typeid(*this).hash_code();
}

ComponentLadder::ComponentLadder(dae::GameObject* pParent, const CollisionBox& collisionBox, bool canClimbUp, bool canClimbDown)
	:ComponentBase(pParent)
	,m_CollisionBox{collisionBox}
	,m_CanClimbUp{canClimbUp}
	,m_CanClimbDown{canClimbDown}
{
	//CollisionManager::GetInstance().AddLadder(this);
	if (m_CanClimbUp && m_CanClimbDown)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladder), this);
	else if (m_CanClimbUp)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladderUpward), this);
	else if (m_CanClimbDown)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladderDownward), this);
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
