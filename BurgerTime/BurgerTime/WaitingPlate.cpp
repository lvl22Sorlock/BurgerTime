//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "WaitingPlate.h"
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

std::size_t WaitingPlate::GetTypeHash()
{
	return typeid(*this).hash_code();
}

WaitingPlate::WaitingPlate(dae::GameObject* pParent, const CollisionBox& collisionBox)
	: ComponentBase(pParent)
	, m_CollisionBox{ collisionBox }
{
	CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::waitingPlate), this);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void WaitingPlate::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}