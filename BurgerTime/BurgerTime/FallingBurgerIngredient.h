#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"
namespace dae
{
	class Scene;
}
class IngredientPart;
namespace SimonGlobalEnums
{
	enum class IngredientType;
}
#include "Observer.h"

//-------------------------------------------------------------------------
//	FallingBurgerIngredient Class
//-------------------------------------------------------------------------
class FallingBurgerIngredient final : public ComponentBase, public Observer
{
public:
	FallingBurgerIngredient(dae::GameObject* pParent, const CollisionBox& collisionBox, SimonGlobalEnums::IngredientType type, dae::Scene& scene);
	~FallingBurgerIngredient() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	FallingBurgerIngredient(const FallingBurgerIngredient& other) = delete;
	FallingBurgerIngredient(FallingBurgerIngredient&& other) noexcept = delete;
	FallingBurgerIngredient& operator=(const FallingBurgerIngredient& other) = delete;
	FallingBurgerIngredient& operator=(FallingBurgerIngredient&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Render() const override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void OnNotify(Event event, void* pData = nullptr) override;

	virtual bool IsCollidingWithOther(const CollisionBox& otherCollisionBox) const override { return m_CollisionBox.IsColliding(otherCollisionBox); };

	bool IsOnPlate() const;
	virtual std::size_t GetTypeHash() override;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	void InitializeParts(dae::Scene& scene);
	void SetIsFalling(bool isFalling);
	void SetIsOnPlate();

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	CollisionBox m_CollisionBox;
	SimonGlobalEnums::IngredientType m_IngredientType;
	std::vector<IngredientPart*> m_PartPtrs;
	int m_NrDroppedParts;
	const float m_FALLING_SPEED;
	bool m_IsFalling;
	bool m_WasCollidingWithOtherIngredient;
	const float m_COLLIDE_WITH_OTHER_INGREDIENT_COOLDOWN;
	float m_CurrentIngredientCollideCooldown;

	bool m_IsOnPlate;
};
