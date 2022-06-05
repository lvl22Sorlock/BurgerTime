#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"

//-------------------------------------------------------------------------
//	Platform Class
//-------------------------------------------------------------------------
class Platform final : public ComponentBase
{
public:
	Platform(dae::GameObject* pParent, const CollisionBox& collisionBox, bool onlyOneDirection = false, bool isOnlyDirectionLeft = true);
	~Platform() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	Platform(const Platform& other) = delete;
	Platform(Platform&& other) noexcept = delete;
	Platform& operator=(const Platform& other) = delete;
	Platform& operator=(Platform&& other)	noexcept = delete;

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
