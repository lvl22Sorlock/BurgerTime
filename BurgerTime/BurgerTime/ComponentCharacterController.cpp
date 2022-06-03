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
	, m_LadderPos{0,0}
	, m_WidthHeight{widthHeight}
	, m_CollisionBox({ pParent->GetPosition().x, pParent->GetPosition().y}, widthHeight.x, widthHeight.y)
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
	m_IsMoving = true;

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
	m_IsMovingRight = m_MoveDirection.x >= 0;

	// calculate final movement direction
	bool isGoingDown{ m_MoveDirection.y > SMALL_FLOAT }; // because down has positive y axis
	bool isGoingUp{ m_MoveDirection.y < -SMALL_FLOAT };
	if (m_IsOnUpwardLadder && isGoingUp)
	{		// climb ladder
		m_pParentGameObject->SetPosition(m_LadderPos.x, m_pParentGameObject->GetPosition().y);
		m_MoveDirection.x = 0;
	}	
	else if (m_IsOnDownwardLadder && isGoingDown)
	{		// descend ladder
		m_pParentGameObject->SetPosition(m_LadderPos.x, m_pParentGameObject->GetPosition().y);
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
	}

	if (right != ' ')
	{
		Command* pMoveRightCommand{ new MoveRightCommand(this) };
		inputManager.AddCommandToCommandList(pMoveRightCommand);
		inputManager.AddCommandToButton(right, InputType::IsDown, pMoveRightCommand);
	}

	if (up != ' ')
	{
		Command* pMoveUpCommand{ new MoveUpCommand(this) };
		inputManager.AddCommandToCommandList(pMoveUpCommand);
		inputManager.AddCommandToButton(up, InputType::IsDown, pMoveUpCommand);
	}

	if (down != ' ')
	{
		Command* pMoveDownCommand{ new MoveDownCommand(this) };
		inputManager.AddCommandToCommandList(pMoveDownCommand);
		inputManager.AddCommandToButton(down, InputType::IsDown, pMoveDownCommand);
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
