//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "MiniginPCH.h"
#include "CollisionManager.h"

#include "ComponentBase.h"

//#include "ComponentLadder.h"
//#include "ComponentCharacterController.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
//CollisionManager::CollisionManager()
//{
//
//}
//
//CollisionManager::~CollisionManager()
//{
//
//}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------


//void CollisionManager::AddLadder(const ComponentLadder* pLadder)
//{
//	m_LadderPtrs.push_back(pLadder);	// emplace_back
//}
//
//void CollisionManager::AddPlayer(const ComponentCharacterController* pPlayer)
//{
//	m_PlayerPtrs.push_back(pPlayer);	// emplace_back
//}
//
//bool CollisionManager::IsCollidingWithLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
//{
//	for (const ComponentLadder* pLadder : m_LadderPtrs)
//	{
//		if (collisionBox.IsColliding(pLadder->GetCollisionBox()))
//		{
//			ladderXPosOut = pLadder->GetPos();
//			return true;
//		}
//	}
//	return false;
//}
//
//bool CollisionManager::IsCollidingWithUpLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
//{
//	for (const ComponentLadder* pLadder : m_LadderPtrs)
//	{
//		if (pLadder->GetGoesUp() && collisionBox.IsColliding(pLadder->GetCollisionBox()))
//		{
//			ladderXPosOut = pLadder->GetPos();
//			return true;
//		}
//	}
//	return false;
//}

//bool CollisionManager::IsCollidingWithDownLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
//{
//	for (const ComponentLadder* pLadder : m_LadderPtrs)
//	{
//		if (pLadder->GetGoesDown() && collisionBox.IsColliding(pLadder->GetCollisionBox()))
//		{
//			ladderXPosOut = pLadder->GetPos();
//			return true;
//		}
//	}
//	return false;
//}

bool CollisionManager::IsCollidingWithObjectOfTag(const std::string& objectTag, const CollisionBox& collisionBox, ComponentBase** pOtherComponentPtr) const
{
	if (m_CollidingObjectVectorsByTagMap.find(objectTag) == m_CollidingObjectVectorsByTagMap.end()) {
		return false;
	}
	for (ComponentBase* pOtherComponent : m_CollidingObjectVectorsByTagMap.at(objectTag))
	{
		if (pOtherComponent->IsCollidingWithOther(collisionBox)) {
			*pOtherComponentPtr = pOtherComponent;
			return true;
		}
	}
	return false;
}

void CollisionManager::AddCollidingObject(const std::string& tag, ComponentBase* pObject)
{
	if (m_CollidingObjectVectorsByTagMap.find(tag) == m_CollidingObjectVectorsByTagMap.end())
		m_CollidingObjectVectorsByTagMap[tag] = std::vector<ComponentBase*>{};

	m_CollidingObjectVectorsByTagMap[tag].emplace_back(pObject);
}
