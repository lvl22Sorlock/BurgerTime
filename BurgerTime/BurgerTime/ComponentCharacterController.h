#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include "Command.hpp"
#include "StructsEnums.h"

namespace dae
{
	class InputManager;
}

class ComponentSpriteRenderer;

namespace SimonGlobalEnums
{
	enum class CharacterType;
}


//-------------------------------------------------------------------------
//	ComponentCharacterController Class
//-------------------------------------------------------------------------
class ComponentCharacterController final : public ComponentBase
{
public:
	ComponentCharacterController(dae::GameObject* pParent, const Vector2<float>& widthHeight, bool isPlayer = false);
	~ComponentCharacterController();

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	ComponentCharacterController(const ComponentCharacterController& other) = delete;
	ComponentCharacterController(ComponentCharacterController&& other) noexcept = delete;
	ComponentCharacterController& operator=(const ComponentCharacterController& other) = delete;
	ComponentCharacterController& operator=(ComponentCharacterController&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void FixedUpdate(float deltaTime) override;
	virtual void Render() const override;
	virtual bool IsCollidingWithOther(const CollisionBox& otherCollisionBox) const override { return m_CollisionBox.IsColliding(otherCollisionBox); };


	void InitializeMovementInput(dae::InputManager& inputManager, int playerNum = 0);
	void MoveCharacter(const Vector2<float>& moveDirection);

	void SetIsMovingAnim(bool* pIsMoving);
	void SetIsMovingRightAnim(bool* pIsMovingRight);
	void SetControllerType(SimonGlobalEnums::CharacterType controllerType);
	void SetSpriteRenderer(ComponentSpriteRenderer* pSpriteRenderer);

	const bool* GetIsOnLadderPtr() const;
	const bool* GetIsOnUpwardsLadderPtr() const;
	const bool* GetIsOnDownwardsLadderPtr() const;
	const bool* GetIsOnPlatformPtr() const;
	const bool* GetIsOnLeftPlatformPtr() const;
	const bool* GetIsOnRightPlatformPtr() const;

private:
	enum class AnimationType
	{
		walking,
		ladderUp,
		ladderDown
	};
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;

	void CalculateMovement(float deltaTime);
	void SetAnimationVariables();
	void CalculateCollisionBox();
	Vector2<float> GetRoundedPos(const Vector2<float>& pos) const;
	void SetAnimation(AnimationType animationType);

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	SimonGlobalEnums::CharacterType m_CharacterType;

	//// Collision
	Vector2<float> m_WidthHeight;
	CollisionBox m_CollisionBox;

	//// Movement
	bool m_IsMoving;
	Vector2<float> m_MoveDirection;
	bool m_IsMovingRight;
	const Vector2<float> m_MOVEMENT_SPEED;

	// Ladders
	bool m_IsOnLadder;
	bool m_IsOnUpwardLadder;
	bool m_IsOnDownwardLadder;
	float m_LadderXPos;
	
	// Platforms
	bool m_IsOnPlatform;
	bool m_IsOnLeftPlatform;
	bool m_IsOnRightPlatform;

	//// Animation
	bool* m_pIsMovingAnimation;
	bool* m_pIsMovingRightAnimation;
	ComponentSpriteRenderer* m_pSpriteRenderer;
	const std::wstring m_WALKING_ANIM_NAME;
	const std::wstring m_LADDER_UP_ANIM_NAME;
	const std::wstring m_LADDER_DOWN_ANIM_NAME;
	AnimationType m_CurrentAnimation;

	//// Attacking
	int m_RemainingPepperAttacks;
};


//-------------------------------------------------------------------------
//	Commands
//-------------------------------------------------------------------------

class MoveLeftCommand : public Command
{
public:
	MoveLeftCommand(ComponentCharacterController* pController)
		:Command()
		, m_pController{ pController }
	{}

	void Execute() override
	{
		if (!m_pController) return;
		m_pController->MoveCharacter({ -1,0 });
	}

protected:
	ComponentCharacterController* m_pController;
};
class MoveRightCommand : public Command
{
public:
	MoveRightCommand(ComponentCharacterController* pController)
		:Command()
		, m_pController{ pController }
	{}

	void Execute() override
	{
		if (!m_pController) return;
		m_pController->MoveCharacter({ 1,0 });
	}

protected:
	ComponentCharacterController* m_pController;
};
class MoveUpCommand : public Command
{
public:
	MoveUpCommand(ComponentCharacterController* pController)
		:Command()
		, m_pController{ pController }
	{}

	void Execute() override
	{
		if (!m_pController) return;
		m_pController->MoveCharacter({ 0,-1 });
	}

protected:
	ComponentCharacterController* m_pController;
};
class MoveDownCommand : public Command
{
public:
	MoveDownCommand(ComponentCharacterController* pController)
		:Command()
		, m_pController{ pController }
	{}

	void Execute() override
	{
		if (!m_pController) return;
		m_pController->MoveCharacter({ 0,1 });
	}

protected:
	ComponentCharacterController* m_pController;
};