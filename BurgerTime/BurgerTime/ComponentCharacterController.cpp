//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "ComponentCharacterController.h"
#include <algorithm>
#include "InputManager.h"
#include "CollisionManager.h"
#include "BurgerTimeGlobal.h"
#include "DebugManager.h"

using namespace dae;
using namespace SimonGlobalConstants;
using namespace SimonGlobalEnums;
using namespace SimonGlobalFunctions;

#include "StructsEnums.h"
using namespace inputEnums;


//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t ComponentCharacterController::GetTypeHash()
{
	return typeid(*this).hash_code();
}

ComponentCharacterController::ComponentCharacterController(dae::GameObject* pParent, const Vector2<float>& widthHeight)
	:ComponentBase(pParent)
	// Movement
	, m_MoveDirection()
	, m_MOVEMENT_SPEED{125.f, 125.f}
	, m_IsMoving{false}
	, m_pIsMovingAnimation{nullptr}
	, m_pIsMovingRightAnimation{nullptr}
	, m_IsMovingRight{false}
	, m_IsOnLadder{false}
	, m_IsOnUpwardLadder{false}
	, m_IsOnDownwardLadder{false}
	, m_LadderXPos{0}
	, m_WidthHeight{widthHeight}
	, m_CollisionBox({ pParent->GetPosition().x, pParent->GetPosition().y}, widthHeight.x, widthHeight.y)
	, m_IsOnPlatform{false}
	, m_IsOnLeftPlatform{false}
	, m_IsOnRightPlatform{false}
{}

ComponentCharacterController::~ComponentCharacterController()
{

}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void ComponentCharacterController::Update(float deltaTime)
{
	SetAnimationVariables();
	CalculateCollisionBox();

	ComponentBase* pLadderComponent{};
	// ladder collision
	m_IsOnUpwardLadder 
		= CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::ladderUpward), m_CollisionBox, &pLadderComponent)
		|| CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::ladder), m_CollisionBox, &pLadderComponent);
	m_IsOnDownwardLadder 
		= CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::ladderDownward), m_CollisionBox, &pLadderComponent)
		|| CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::ladder), m_CollisionBox, &pLadderComponent);
	m_IsOnLadder = m_IsOnUpwardLadder || m_IsOnDownwardLadder;
	if (pLadderComponent)
		m_LadderXPos = pLadderComponent->GetParent()->GetPosition().x;
	// platform collision
	m_IsOnPlatform = CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::platform), m_CollisionBox, nullptr);
	m_IsOnLeftPlatform = CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::platformLeft), m_CollisionBox, nullptr);
	m_IsOnRightPlatform = CollisionManager::GetInstance().IsCollidingWithObjectOfTag(static_cast<int32_t>(CollisionTag::platformRight), m_CollisionBox, nullptr);

	if (m_IsMoving)
	{
		CalculateMovement(deltaTime);
		m_IsMoving = false;
	}
}

void ComponentCharacterController::Render() const
{
	DebugManager::GetInstance().RenderDebugSquare(m_pParentGameObject->GetPosition(), m_CollisionBox.GetWidthHeight());
}

void ComponentCharacterController::MoveCharacter(const Vector2<float>& moveDirection)
{
	//if (std::abs(moveDirection.x) >= 0.1f || std::abs(moveDirection.y) >= 1.0f)
	m_IsMoving = true;
	std::cout << moveDirection.x << ' ' << moveDirection.y << std::endl;

	if (m_IsOnPlatform
		||
		m_IsOnLeftPlatform && moveDirection.x <= 0
		||
		m_IsOnRightPlatform && moveDirection.x >= 0)
		m_MoveDirection.x += moveDirection.x;
	//m_MoveDirection.x = std::clamp(m_MoveDirection.x + moveDirection.x, -1.0f, 1.0f);

	if (m_IsOnLadder)
	{
		m_MoveDirection.y += moveDirection.y;
		//m_MoveDirection.y = std::clamp(m_MoveDirection.y + moveDirection.y, -1.0f, 1.0f);
	}

}

void ComponentCharacterController::CalculateMovement(float deltaTime)
{
	if (!m_IsOnLadder)
		m_MoveDirection.y = 0;

	// Clamp Movement
	const float SMALL_FLOAT{ 0.01f };
	if (std::abs(m_MoveDirection.x) < SMALL_FLOAT && std::abs(m_MoveDirection.y) < SMALL_FLOAT) return;
	m_MoveDirection.x = std::clamp(m_MoveDirection.x, -1.0f, 1.0f);
	m_MoveDirection.y = std::clamp(m_MoveDirection.y, -1.0f, 1.0f);
	if (m_MoveDirection.x > EPSILON)
		m_IsMovingRight = true;
	if (m_MoveDirection.x < -EPSILON)
		m_IsMovingRight = false;

	// calculate final movement direction
	bool isGoingDown{ m_MoveDirection.y > SMALL_FLOAT }; // because down has positive y axis
	bool isGoingUp{ m_MoveDirection.y < -SMALL_FLOAT };
	if (m_IsOnUpwardLadder && isGoingUp)
	{		// climb ladder
		m_pParentGameObject->SetPosition(m_LadderXPos, m_pParentGameObject->GetPosition().y);
		m_MoveDirection.x = 0;
	}	
	else if (m_IsOnDownwardLadder && isGoingDown)
	{		// descend ladder
		m_pParentGameObject->SetPosition(m_LadderXPos, m_pParentGameObject->GetPosition().y);
		m_MoveDirection.x = 0;
	}
	else	// walk || leave ladder
	{
		m_MoveDirection.y = 0;

		if (m_IsOnLadder)
		{
			m_pParentGameObject->SetPosition(m_pParentGameObject->GetPosition().x, GetRoundedPos(m_pParentGameObject->GetPosition()).y);
			//m_pParentGameObject->SetPosition(m_pParentGameObject->GetPosition().x, m_LadderPos.y);
		}
	}

	// Move
	m_pParentGameObject->Move(
		{ m_MoveDirection.x * m_MOVEMENT_SPEED.x * deltaTime,
		 m_MoveDirection.y * m_MOVEMENT_SPEED.y * deltaTime }
	);

	// Reset Movement Direction
	m_MoveDirection = { 0,0 };
}

void ComponentCharacterController::InitializeMovementInput(InputManager& inputManager, char left, char right, char up, char down)
{
	if (left != ' ')
	{
		Command* pMoveLeftCommand{ new MoveLeftCommand(this) };
		inputManager.AddCommandToCommandList(pMoveLeftCommand);
		inputManager.AddCommandToButton(left, InputType::IsDown, pMoveLeftCommand);

		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftLeft, pMoveLeftCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightLeft, pMoveLeftCommand, 1);
		//inputManager.AddCommandToButton(ControllerButton::DPadLeft, InputType::IsDown, pMoveLeftCommand, 1);
		//inputManager.AddCommandToButton(ControllerButton::DPadLeft, InputType::IsDown, pMoveLeftCommand, 2);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftLeft, pMoveLeftCommand, 2);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightLeft, pMoveLeftCommand, 2);
	}

	if (right != ' ')
	{
		Command* pMoveRightCommand{ new MoveRightCommand(this) };
		inputManager.AddCommandToCommandList(pMoveRightCommand);
		inputManager.AddCommandToButton(right, InputType::IsDown, pMoveRightCommand);

		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftRight, pMoveRightCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightRight, pMoveRightCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftRight, pMoveRightCommand, 2);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightRight, pMoveRightCommand, 2);
		//inputManager.AddCommandToButton(ControllerButton::DPadRight, InputType::IsDown, pMoveRightCommand, 1);
		//inputManager.AddCommandToButton(ControllerButton::DPadRight, InputType::IsDown, pMoveRightCommand, 2);
	}

	if (up != ' ')
	{
		Command* pMoveUpCommand{ new MoveUpCommand(this) };
		inputManager.AddCommandToCommandList(pMoveUpCommand);
		inputManager.AddCommandToButton(up, InputType::IsDown, pMoveUpCommand);

		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftUp, pMoveUpCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightUp, pMoveUpCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftUp, pMoveUpCommand, 2);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightUp, pMoveUpCommand, 2);
		//inputManager.AddCommandToButton(ControllerButton::DPadUp, InputType::IsDown, pMoveUpCommand, 1);
		//inputManager.AddCommandToButton(ControllerButton::DPadUp, InputType::IsDown, pMoveUpCommand, 2);
	}

	if (down != ' ')
	{
		Command* pMoveDownCommand{ new MoveDownCommand(this) };
		inputManager.AddCommandToCommandList(pMoveDownCommand);
		inputManager.AddCommandToButton(down, InputType::IsDown, pMoveDownCommand);

		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftDown, pMoveDownCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightDown, pMoveDownCommand, 1);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftDown, pMoveDownCommand, 2);
		//inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightDown, pMoveDownCommand, 2);
		//inputManager.AddCommandToButton(ControllerButton::DPadDown, InputType::IsDown, pMoveDownCommand, 1);
		//inputManager.AddCommandToButton(ControllerButton::DPadDown, InputType::IsDown, pMoveDownCommand, 2);
	}
}

void ComponentCharacterController::SetAnimationVariables()
{
	if (m_pIsMovingAnimation)
		*m_pIsMovingAnimation = m_IsMoving;
	if (m_pIsMovingRightAnimation)
		*m_pIsMovingRightAnimation = m_IsMovingRight;
}

void ComponentCharacterController::SetIsMovingAnim(bool* pIsMoving)
{
	m_pIsMovingAnimation = pIsMoving;
}
void ComponentCharacterController::SetIsMovingRightAnim(bool* pIsMovingRight)
{
	m_pIsMovingRightAnimation = pIsMovingRight;
}

void ComponentCharacterController::CalculateCollisionBox()
{
	Vector2<float> position{ m_pParentGameObject->GetPosition().x, m_pParentGameObject->GetPosition().y };
	m_CollisionBox.leftBottom = position;
	m_CollisionBox.rightTop = position + m_WidthHeight;
}

Vector2<float> ComponentCharacterController::GetRoundedPos(const Vector2<float>& pos) const
{
	// temp code
	return { CELL_WIDTH * std::roundf(pos.x/ CELL_WIDTH), CELL_WIDTH * std::roundf(pos.y / CELL_WIDTH) };
}
