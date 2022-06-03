#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
using namespace dae;

//-------------------------------------------------------------------------
//	ComponentFollowOther Class
//-------------------------------------------------------------------------
class ComponentFollowOther final : public ComponentBase
{
public:
	ComponentFollowOther(dae::GameObject* pParent, dae::GameObject* pTarget);
	~ComponentFollowOther() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	ComponentFollowOther(const ComponentFollowOther& other) = delete;
	ComponentFollowOther(ComponentFollowOther&& other) noexcept = delete;
	ComponentFollowOther& operator=(const ComponentFollowOther& other) = delete;
	ComponentFollowOther& operator=(ComponentFollowOther&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void Update(float deltaTime) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
	
	GameObject* m_pTarget;
};
