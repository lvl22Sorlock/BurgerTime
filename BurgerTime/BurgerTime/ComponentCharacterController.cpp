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

#include "ComponentSpriteRenderer.h"


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

ComponentCharacterController::ComponentCharacterController(dae::GameObject* pParent, const Vector2<float>& widthHeight, bool isPlayer)
	:ComponentBase(pParent)
	// Movement
	, m_MoveDirection()
	, m_MOVEMENT_SPEED{100.f, 75.f}
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
	, m_RemainingPepperAttacks{5}
	, m_pSpriteRenderer{nullptr}
	, m_WALKING_ANIM_NAME{L"WalkHorizontal"}
	, m_LADDER_UP_ANIM_NAME{L"LadderUp"}
	, m_LADDER_DOWN_ANIM_NAME{L"LadderDown"}
	, m_CurrentAnimation{-1}
	, m_CharacterType{ CharacterType::player}
{
	if (isPlayer)
		CollisionManager::GetInstance().AddCollidingObject(static_cast<int32_t>(CollisionTag::player), this);
}

ComponentCharacterController::~ComponentCharacterController()
{

}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void ComponentCharacterController::FixedUpdate(float deltaTime)
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
	if (std::abs(moveDirection.x) >= EPSILON 
		||
		std::abs(moveDirection.y) >= EPSILON)
	{
		m_IsMoving = true;
		std::cout << moveDirection.x << ' ' << moveDirection.y << std::endl;
	}
	//std::cout << moveDirection.x << ' ' << moveDirection.y << std::endl;

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
		SetAnimation(AnimationType::ladderUp);
	}	
	else if (m_IsOnDownwardLadder && isGoingDown)
	{		// descend ladder
		m_pParentGameObject->SetPosition(m_LadderXPos, m_pParentGameObject->GetPosition().y);
		m_MoveDirection.x = 0;
		SetAnimation(AnimationType::ladderDown);
	}
	else	// walk || leave ladder
	{
		m_MoveDirection.y = 0;
		SetAnimation(AnimationType::walking);

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

void ComponentCharacterController::InitializeMovementInput(InputManager& inputManager, int playerNum)
{
	const char left{ 'a' };
	const char right{ 'd' };
	const char up{ 'w' };
	const char down{ 's' };
	//if (left != ' ')
	{
		Command* pMoveLeftCommand{ new MoveLeftCommand(this) };
		switch (playerNum)
		{
		case 0:
		case 1:
			inputManager.AddCommandToCommandList(pMoveLeftCommand);
			inputManager.AddCommandToButton(left, InputType::IsDown, pMoveLeftCommand);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftLeft, pMoveLeftCommand, 1);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightLeft, pMoveLeftCommand, 1);
			inputManager.AddCommandToButton(ControllerButton::DPadLeft, InputType::IsDown, pMoveLeftCommand, 1);
			break;
		case 2:
			inputManager.AddCommandToButton(ControllerButton::DPadLeft, InputType::IsDown, pMoveLeftCommand, 2);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftLeft, pMoveLeftCommand, 2);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightLeft, pMoveLeftCommand, 2);
			break;
		default:
			std::cout << "playerNum not recognized" << std::endl;
			break;
		}
	}

	//if (right != ' ')
	{
		Command * pMoveRightCommand{ new MoveRightCommand(this) };
		switch (playerNum)
		{
		case 0:
		case 1:
			inputManager.AddCommandToCommandList(pMoveRightCommand);
			inputManager.AddCommandToButton(right, InputType::IsDown, pMoveRightCommand);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftRight, pMoveRightCommand, 1);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightRight, pMoveRightCommand, 1);
			inputManager.AddCommandToButton(ControllerButton::DPadRight, InputType::IsDown, pMoveRightCommand, 1);
			break;
		case 2:
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightRight, pMoveRightCommand, 2);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftRight, pMoveRightCommand, 2);
			inputManager.AddCommandToButton(ControllerButton::DPadRight, InputType::IsDown, pMoveRightCommand, 2);
			break;
		default:
			break;
		}
	}

	//if (up != ' ')
	{
		Command * pMoveUpCommand{ new MoveUpCommand(this) };
		switch (playerNum)
		{
		case 0:
		case 1:
			inputManager.AddCommandToCommandList(pMoveUpCommand);
			inputManager.AddCommandToButton(up, InputType::IsDown, pMoveUpCommand);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftUp, pMoveUpCommand, 1);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightUp, pMoveUpCommand, 1);
			inputManager.AddCommandToButton(ControllerButton::DPadUp, InputType::IsDown, pMoveUpCommand, 1);
			break;
		case 2:
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightUp, pMoveUpCommand, 2);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftUp, pMoveUpCommand, 2);
			inputManager.AddCommandToButton(ControllerButton::DPadUp, InputType::IsDown, pMoveUpCommand, 2);
			break;
		default:
			break;
		}
	}

	//if (down != ' ')
	{
		Command * pMoveDownCommand{ new MoveDownCommand(this) };
		switch (playerNum)
		{
		case 0:
		case 1:
			inputManager.AddCommandToCommandList(pMoveDownCommand);
			inputManager.AddCommandToButton(down, InputType::IsDown, pMoveDownCommand);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftDown, pMoveDownCommand, 1);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightDown, pMoveDownCommand, 1);
			inputManager.AddCommandToButton(ControllerButton::DPadDown, InputType::IsDown, pMoveDownCommand, 1);
			break;
		case 2:
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::rightDown, pMoveDownCommand, 2);
			inputManager.AddCommandToJoystickDirection(ControllerJoystickDirection::leftDown, pMoveDownCommand, 2);
			inputManager.AddCommandToButton(ControllerButton::DPadDown, InputType::IsDown, pMoveDownCommand, 2);
			break;
		default:
			break;
		}
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

void ComponentCharacterController::SetAnimation(AnimationType animationType)
{
	if (m_pSpriteRenderer == nullptr) return;
	if (m_CurrentAnimation == animationType) return;
	m_CurrentAnimation = animationType;

	switch (animationType)
	{
	case AnimationType::walking:
		m_pSpriteRenderer->TrySetAnimation(m_WALKING_ANIM_NAME);
		break;
	case AnimationType::ladderDown:
		m_pSpriteRenderer->TrySetAnimation(m_LADDER_DOWN_ANIM_NAME);
		break;
	case AnimationType::ladderUp:
		m_pSpriteRenderer->TrySetAnimation(m_LADDER_UP_ANIM_NAME);
		break;
	default:
		std::cout << "CharacterController animationType not found" << std::endl;
		break;
	}
}

void ComponentCharacterController::SetSpriteRenderer(ComponentSpriteRenderer* pSpriteRenderer)
{
	m_pSpriteRenderer = pSpriteRenderer;
	ComponentSpriteRenderer::AnimInfo walkingAnimInfo{};
	ComponentSpriteRenderer::AnimInfo ladderUpAnimInfo{};
	ComponentSpriteRenderer::AnimInfo ladderDownAnimInfo{};

	pSpriteRenderer->SetSpritesheet(SPRITESHEET_PATH);
	switch (m_CharacterType)
	{
	case CharacterType::player:
		walkingAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(48, 0), 1, 3);
		ladderUpAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(96, 0), 1, 3);
		ladderDownAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(0, 0), 1, 3);
		break;
	case CharacterType::mrHotDog:
		walkingAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(32, 32), 1, 2);
		ladderUpAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(64, 32), 1, 2);
		ladderDownAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(0, 32), 1, 2);
		break;
	case CharacterType::mrPickle:
		walkingAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(32, 64), 1, 2);
		ladderUpAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(64, 64), 1, 2);
		ladderDownAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(0, 64), 1, 2);
		break;
	case CharacterType::mrEgg:
		walkingAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(32, 96), 1, 2);
		ladderUpAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(64, 96), 1, 2);
		ladderDownAnimInfo = ComponentSpriteRenderer::AnimInfo(Vector2<int>(0, 96), 1, 2);
		break;
	default:
		std::cout 
			<< "ComponentCharacterController::SetSpriteRenderer ControllerType not recognized: " 
			<< std::to_string(static_cast<int>(m_CharacterType))
			<< std::endl;
		break;
	}
	pSpriteRenderer->AddAnimation(m_WALKING_ANIM_NAME, walkingAnimInfo);
	pSpriteRenderer->TrySetAnimation(m_WALKING_ANIM_NAME);
	pSpriteRenderer->AddAnimation(m_LADDER_UP_ANIM_NAME, ladderUpAnimInfo);
	pSpriteRenderer->AddAnimation(m_LADDER_DOWN_ANIM_NAME, ladderDownAnimInfo);
}

void ComponentCharacterController::SetControllerType(CharacterType characterType)
{
	m_CharacterType = characterType;
}

const bool* ComponentCharacterController::GetIsOnLadderPtr() const
{
	return &m_IsOnLadder;
}
const bool* ComponentCharacterController::GetIsOnUpwardsLadderPtr() const
{
	return &m_IsOnUpwardLadder;
}
const bool* ComponentCharacterController::GetIsOnDownwardsLadderPtr() const
{
	return &m_IsOnDownwardLadder;
}
const bool* ComponentCharacterController::GetIsOnPlatformPtr() const
{
	return &m_IsOnPlatform;
}
const bool* ComponentCharacterController::GetIsOnLeftPlatformPtr() const
{
	return &m_IsOnLeftPlatform;
}
const bool* ComponentCharacterController::GetIsOnRightPlatformPtr() const
{
	return &m_IsOnRightPlatform;
}
