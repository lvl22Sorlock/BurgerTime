#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
#include "StructsEnums.h"
#include <vector>
class ComponentLadder;
class ComponentCharacterController;

using namespace dae;

//-------------------------------------------------------------------------
//	CollisionManager Class
//-------------------------------------------------------------------------
class CollisionManager final : public Singleton<CollisionManager>
{
public:
	CollisionManager();
	~CollisionManager();

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

	void AddLadder(const ComponentLadder* pLadder);
	void AddPlayer(const ComponentCharacterController* pPlayer);

	bool IsCollidingWithLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	bool IsCollidingWithUpLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;
	bool IsCollidingWithDownLadder(const CollisionBox& collisionBox, Vector2<float>& ladderXPosOut) const;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	std::vector<const ComponentLadder*> m_LadderPtrs;
	std::vector<const ComponentCharacterController*> m_PlayerPtrs;


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------




};
