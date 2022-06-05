#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"

//-------------------------------------------------------------------------
//	PepperCloud Class
//-------------------------------------------------------------------------
class PepperCloud final : public ComponentBase
{
public:
	PepperCloud(dae::GameObject* pParent, const CollisionBox& collisionBox);
	~PepperCloud() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	PepperCloud(const PepperCloud& other) = delete;
	PepperCloud(PepperCloud&& other) noexcept = delete;
	PepperCloud& operator=(const PepperCloud& other) = delete;
	PepperCloud& operator=(PepperCloud&& other)	noexcept = delete;

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
