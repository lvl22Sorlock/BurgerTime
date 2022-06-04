//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "PepperCloud.h"
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

std::size_t PepperCloud::GetTypeHash()
{
	return typeid(*this).hash_code();
}

PepperCloud::PepperCloud(dae::GameObject* pParent, const CollisionBox& collisionBox)
	:ComponentBase(pParent)
	, m_CollisionBox{ collisionBox }
{
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::pepperCloud), this);
}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void PepperCloud::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_CollisionBox.leftBottom, m_CollisionBox.GetWidthHeight());
}