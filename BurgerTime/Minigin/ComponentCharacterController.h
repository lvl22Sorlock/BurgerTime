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



//-------------------------------------------------------------------------
//	ComponentCharacterController Class
//-------------------------------------------------------------------------
class ComponentCharacterController final : public ComponentBase
{
public:

	ComponentCharacterController(dae::GameObject* pParent, const Vector2<float>& widthHeight);
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

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;


	void InitializeMovementInput(dae::InputManager& inputManager, char left, char right, char up, char down);
	void MoveCharacter(const Vector2<float>& moveDirection);

	void SetIsMovingAnim(bool* pIsMoving);
	void SetIsMovingRightAnim(bool* pIsMovingRight);

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	void CalculateMovement(float deltaTime);
	void SetAnimationVariables();
	void CalculateCollisionBox();
	Vector2<float> GetRoundedPos(const Vector2<float>& pos) const;


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

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
	Vector2<float> m_LadderPos;

	// Animation
	bool* m_pIsMovingAnimation;
	bool* m_pIsMovingRightAnimation;


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