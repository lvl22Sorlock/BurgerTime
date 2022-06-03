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

	const CollisionBox& GetCollisionBox() const;
	//float GetXPos() const;
	Vector2<float> GetPos() const;
	bool GetGoesUp() const;
	bool GetGoesDown() const;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	CollisionBox m_CollisionBox;
	//float m_XPosLeft;
	bool m_CanClimbUp;
	bool m_CanClimbDown;
};
