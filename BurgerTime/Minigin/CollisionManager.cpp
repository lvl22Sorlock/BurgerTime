//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "MiniginPCH.h"
#include "CollisionManager.h"

#include "ComponentBase.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

bool CollisionManager::IsCollidingWithObjectOfTag(int32_t objectTag, const CollisionBox& collisionBox, ComponentBase** pOtherComponentPtr) const
{
	if (m_CollidingObjectVectorsByTagMap.find(objectTag) == m_CollidingObjectVectorsByTagMap.end()) {
		return false;
	}
	for (ComponentBase* pOtherComponent : m_CollidingObjectVectorsByTagMap.at(objectTag))
	{
		if (pOtherComponent->IsCollidingWithOther(collisionBox)) {
			if (pOtherComponentPtr)
				*pOtherComponentPtr = pOtherComponent;
			return true;
		}
	}
	return false;
}

void CollisionManager::AddCollidingObject(int32_t tag, ComponentBase* pObject)
{
	if (m_CollidingObjectVectorsByTagMap.find(tag) == m_CollidingObjectVectorsByTagMap.end())
		m_CollidingObjectVectorsByTagMap[tag] = std::vector<ComponentBase*>{};

	m_CollidingObjectVectorsByTagMap[tag].emplace_back(pObject);
}
