#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
#include "StructsEnums.h"
#include <vector>
#include <unordered_map>
class ComponentBase;

using namespace dae;

//-------------------------------------------------------------------------
//	CollisionManager Class
//-------------------------------------------------------------------------
class CollisionManager final : public Singleton<CollisionManager>
{
public:
	friend class dae::Singleton<CollisionManager>;
	~CollisionManager() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	CollisionManager(const CollisionManager& other) = delete;
	CollisionManager(CollisionManager&& other) noexcept = delete;
	CollisionManager& operator=(const CollisionManager& other) = delete;
	CollisionManager& operator=(CollisionManager&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------


	void AddCollidingObject(int32_t tag, ComponentBase* pObject);
	bool IsCollidingWithObjectOfTag(int32_t objectTag, const CollisionBox& collisionBox, ComponentBase** pOtherComponentPtr = nullptr, ComponentBase* pCallingObject = nullptr) const;

private:
	CollisionManager() = default;
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	std::unordered_map<int32_t, std::vector<ComponentBase*>> m_CollidingObjectVectorsByTagMap;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------




};
