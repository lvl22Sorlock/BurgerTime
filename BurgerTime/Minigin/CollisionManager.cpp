//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "MiniginPCH.h"
#include "CollisionManager.h"

#include "ComponentLadder.h"
#include "ComponentCharacterController.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------


void CollisionManager::AddLadder(const ComponentLadder* pLadder)
{
	m_LadderPtrs.push_back(pLadder);	// emplace_back
}

void CollisionManager::AddPlayer(const ComponentCharacterController* pPlayer)
{
	m_PlayerPtrs.push_back(pPlayer);	// emplace_back
}

bool CollisionManager::IsCollidingWithLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
{
	for (const ComponentLadder* pLadder : m_LadderPtrs)
	{
		if (collisionBox.IsColliding(pLadder->GetCollisionBox()))
		{
			ladderXPosOut = pLadder->GetPos();
			return true;
		}
	}
	return false;
}

bool CollisionManager::IsCollidingWithUpLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
{
	for (const ComponentLadder* pLadder : m_LadderPtrs)
	{
		if (pLadder->GetGoesUp() && collisionBox.IsColliding(pLadder->GetCollisionBox()))
		{
			ladderXPosOut = pLadder->GetPos();
			return true;
		}
	}
	return false;
}

bool CollisionManager::IsCollidingWithDownLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const
{
	for (const ComponentLadder* pLadder : m_LadderPtrs)
	{
		if (pLadder->GetGoesDown() && collisionBox.IsColliding(pLadder->GetCollisionBox()))
		{
			ladderXPosOut = pLadder->GetPos();
			return true;
		}
	}
	return false;
}
