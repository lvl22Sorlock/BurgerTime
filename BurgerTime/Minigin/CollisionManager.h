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
	//enum class CollidingObjectType
	//{
	//	ladder,
	//	ladderUpwards,
	//	ladderDownwards
	//};

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
	void AddCollidingObject(const std::string& tag, ComponentBase* pObject);

	bool IsCollidingWithObjectOfTag(const std::string& objectTag, const CollisionBox& collisionBox, ComponentBase** pOtherComponentPtr) const;
	//bool IsCollidingWithLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	//bool IsCollidingWithUpLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	//bool IsCollidingWithDownLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;

protected:
	CollisionManager() = default;
private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//std::vector<const ComponentLadder*> m_LadderPtrs;
	//std::vector<const ComponentCharacterController*> m_PlayerPtrs;
	std::unordered_map<std::string, std::vector<ComponentBase*>> m_CollidingObjectVectorsByTagMap;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------




};
