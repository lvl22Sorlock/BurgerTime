//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "Platform.h"
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

std::size_t Platform::GetTypeHash()
{
	return typeid(*this).hash_code();
}

Platform::Platform(dae::GameObject* pParent, const CollisionBox& collisionBox, bool onlyOneDirection, bool isOnlyDirectionLeft)
	: ComponentBase(pParent)
	, m_CollisionBox{ collisionBox }
{
	if (!onlyOneDirection)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::platform), this);
	else if (onlyOneDirection && isOnlyDirectionLeft)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::platformLeft), this);
	else if (onlyOneDirection && !isOnlyDirectionLeft)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::platformRight), this);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void Platform::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}