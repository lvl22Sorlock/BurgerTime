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
	//,m_CanClimbUp{canClimbUp}
	//,m_CanClimbDown{canClimbDown}
{
	//CollisionManager::GetInstance().AddLadder(this);
	if (canClimbUp && canClimbDown)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladder), this);
	else if (canClimbUp)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladderUpward), this);
	else if (canClimbDown)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::ladderDownward), this);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void ComponentLadder::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}