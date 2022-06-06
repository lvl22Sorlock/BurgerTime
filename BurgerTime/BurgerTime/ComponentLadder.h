#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"

//-------------------------------------------------------------------------
//	ComponentLadder Class
//-------------------------------------------------------------------------
class ComponentLadder final : public ComponentBase
{
public:
	ComponentLadder(dae::GameObject* pParent, const CollisionBox& collisionBox, bool canClimbUp = true, bool canClimbDown = true);
	~ComponentLadder() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	ComponentLadder(const ComponentLadder& other) = delete;
	ComponentLadder(ComponentLadder&& other) noexcept = delete;
	ComponentLadder& operator=(const ComponentLadder& other) = delete;
	ComponentLadder& operator=(ComponentLadder&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Render() const override;

	virtual bool IsCollidingWithOther(const CollisionBox& otherCollisionBox) const override { return m_CollisionBox.IsColliding(otherCollisionBox); };

	virtual std::size_t GetTypeHash() override;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	CollisionBox m_CollisionBox;
};
