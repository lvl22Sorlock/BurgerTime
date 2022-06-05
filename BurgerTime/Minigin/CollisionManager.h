#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
#include "StructsEnums.h"
#include <vector>
#include <unordered_map>
//class ComponentLadder;
//class ComponentCharacterController;
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

	//void AddLadder(const ComponentLadder* pLadder);
	//void AddPlayer(const ComponentCharacterController* pPlayer);
	void AddCollidingObject(int32_t tag, ComponentBase* pObject);

	bool IsCollidingWithObjectOfTag(int32_t objectTag, const CollisionBox& collisionBox, ComponentBase** pOtherComponentPtr = nullptr, ComponentBase* pCallingObject = nullptr) const;
	//bool IsCollidingWithLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	//bool IsCollidingWithUpLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	//bool IsCollidingWithDownLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;

private:
	CollisionManager() = default;
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//std::vector<const ComponentLadder*> m_LadderPtrs;
	//std::vector<const ComponentCharacterController*> m_PlayerPtrs;
	std::unordered_map<int32_t, std::vector<ComponentBase*>> m_CollidingObjectVectorsByTagMap;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------




};
