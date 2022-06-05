#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"
namespace SimonGlobalEnums
{
	enum class IngredientType;
}
#include "Subject.h"

//-------------------------------------------------------------------------
//	IngredientPart Class
//-------------------------------------------------------------------------
class IngredientPart final : public ComponentBase, public Subject
{
public:
	IngredientPart(dae::GameObject* pParent, const CollisionBox& collisionBox);
	~IngredientPart() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	IngredientPart(const IngredientPart& other) = delete;
	IngredientPart(IngredientPart&& other) noexcept = delete;
	IngredientPart& operator=(const IngredientPart& other) = delete;
	IngredientPart& operator=(IngredientPart&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Render() const override;
	virtual void Update(float deltaTime) override;

	void SetIsDroppedFalse();

	virtual bool IsCollidingWithOther(const CollisionBox& otherCollisionBox) const override { return m_CollisionBox.IsColliding(otherCollisionBox); };

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	CollisionBox m_CollisionBox;
	bool m_WasCollidingWithPlayer;
	bool m_IsDropped;
	const float m_DROPPED_POS_Y_OFFSET;
};
