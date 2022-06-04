#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "StructsEnums.h"
class ComponentCharacterController;

namespace SimonGlobalEnums
{
	enum class CharacterType;
}

//-------------------------------------------------------------------------
//	EnemyController Class
//-------------------------------------------------------------------------
class EnemyController final : public ComponentBase
{
public:
	EnemyController(dae::GameObject* pParent, SimonGlobalEnums::CharacterType enemyType);
	~EnemyController() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	EnemyController(const EnemyController& other) = delete;
	EnemyController(EnemyController&& other) noexcept = delete;
	EnemyController& operator=(const EnemyController& other) = delete;
	EnemyController& operator=(EnemyController&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Update(float) override;

	void SetCharacterController(ComponentCharacterController* pCharacterController);
	static void SetPlayerPtrs(const std::vector<const ComponentCharacterController*>& playerPtrs);

	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; }

private:
	enum class DirectionHorizontal
	{
		left,
		right
	};
	enum class DirectionVertical
	{
		up,
		down
	};
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;
	Vector2<float> GetDirectionToNearestPlayer() const;
	void TryChooseNewHorizontalDirection(float toPlayerDirection);
	void TryChooseNewVerticalDirection(float toPlayerDirection);
	void CalculateNewCanChangeHorizontalDirection();
	void CalculateNewCanChangeVerticalDirection();

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	SimonGlobalEnums::CharacterType m_EnemyType;

	ComponentCharacterController* m_pCharacterController;
	static std::vector<const ComponentCharacterController*> m_PlayerPtrs;

	const bool* m_pIsOnLadder;
	bool m_WasOnLadder;
	bool m_CanChangeVerticalDirection;
	const bool* m_pIsOnUpwardsLadder;
	const bool* m_pIsOnDownwardsLadder;
	const bool* m_pIsOnPlatform;
	bool m_WasOnPlatform;
	bool m_CanChangeHorizontalDirection;
	const bool* m_pIsOnLeftPlatform;
	const bool* m_pIsOnRightPlatform;
	Vector2<float> m_CurrentDirection;
	bool m_HasInitializedDirection;
};

