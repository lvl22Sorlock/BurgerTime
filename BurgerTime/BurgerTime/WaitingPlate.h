#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"

//-------------------------------------------------------------------------
//	WaitingPlate Class
//-------------------------------------------------------------------------
class WaitingPlate final : public ComponentBase
{
public:
	WaitingPlate(dae::GameObject* pParent, const CollisionBox& collisionBox);
	~WaitingPlate() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	WaitingPlate(const WaitingPlate& other) = delete;
	WaitingPlate(WaitingPlate&& other) noexcept = delete;
	WaitingPlate& operator=(const WaitingPlate& other) = delete;
	WaitingPlate& operator=(WaitingPlate&& other)	noexcept = delete;

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
