//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "EnemyController.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "ComponentCharacterController.h"
#include "BurgerTimeGlobal.h"
using namespace SimonGlobalConstants;
#include <algorithm>

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------
std::vector<const ComponentCharacterController*> EnemyController::m_PlayerPtrs{};

//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------

std::size_t EnemyController::GetTypeHash()
{
	return typeid(*this).hash_code();
}

EnemyController::EnemyController(dae::GameObject* pParent, SimonGlobalEnums::CharacterType enemyType, bool canRandomlyChangeDirection)
	: ComponentBase(pParent)
	, m_pCharacterController{nullptr}
	, m_EnemyType{enemyType}

	, m_pIsOnLadder{nullptr}
	, m_pIsOnDownwardsLadder{nullptr}
	, m_pIsOnUpwardsLadder{nullptr}
	, m_pIsOnPlatform{nullptr}
	, m_pIsOnLeftPlatform{ nullptr }
	, m_pIsOnRightPlatform{ nullptr }

	, m_CurrentDirection{}
	, m_WasOnLadder{false}
	, m_WasOnPlatform{false}
	, m_CanChangeHorizontalDirection{true}
	, m_CanChangeVerticalDirection{true}
	, m_HasInitializedDirection{false}

	, m_CanRandomlyChangeDirection{canRandomlyChangeDirection}
	, m_MIN_RANDOM_DIRECTION_CHANGE_TIME{3.0f}
	, m_MAX_RANDOM_DIRECTION_CHANGE_TIME{10.0f}
	, m_CurrentRandomDirectionChangeCooldown{3.0f}
	, m_CurrentNewDirection{Direction::up}
	, m_WasOnLeftPlatform{false}
	, m_WasOnRightPlatform{false}
{}
//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void EnemyController::SetCharacterController(ComponentCharacterController* pCharacterController)
{
	m_pCharacterController = pCharacterController;
	m_pCharacterController->SetControllerType(m_EnemyType);
	m_pIsOnLadder				= m_pCharacterController->GetIsOnLadderPtr();
	m_pIsOnDownwardsLadder		= m_pCharacterController->GetIsOnDownwardsLadderPtr();
	m_pIsOnUpwardsLadder		= m_pCharacterController->GetIsOnUpwardsLadderPtr();
	m_pIsOnPlatform				= m_pCharacterController->GetIsOnPlatformPtr();
	m_pIsOnLeftPlatform			= m_pCharacterController->GetIsOnLeftPlatformPtr();
	m_pIsOnRightPlatform		= m_pCharacterController->GetIsOnRightPlatformPtr();
}

void EnemyController::SetPlayerPtrs(const std::vector<const ComponentCharacterController*>& playerPtrs)
{
	m_PlayerPtrs = playerPtrs;
}

Vector2<float> EnemyController::GetDirectionToNearestPlayer() const
{
	if (m_PlayerPtrs.size() <= 0) return { 0,0 };
	std::vector<Vector2<float>> playerPositions{};
	playerPositions.reserve(m_PlayerPtrs.size());
	for (const ComponentCharacterController* pPlayer : m_PlayerPtrs)
	{
		playerPositions.emplace_back(pPlayer->GetParent()->GetPosition());
	}

	Vector2<float> closestPlayerPos{playerPositions[0]};
	const Vector2<float> ENEMY_POS{ m_pParentGameObject->GetPosition() };
	auto getDistanceSquared
	{
		[](const Vector2<float> pos1, const Vector2<float>pos2) 
		{
			return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y);
		} 	
	};
	for (const Vector2<float>& playerPos : playerPositions)
	{
		if (getDistanceSquared(playerPos, ENEMY_POS) < getDistanceSquared(closestPlayerPos, ENEMY_POS))
			closestPlayerPos = playerPos;
	}

	Vector2<float> directionToPlayer{ closestPlayerPos - ENEMY_POS};
	if (std::abs(directionToPlayer.x) < 0.1f) directionToPlayer.x = 0;
	//else
	//	directionToPlayer.x = directionToPlayer.x / std::abs(directionToPlayer.x) * 1.0f;

	if (std::abs(directionToPlayer.y) < 0.1f) directionToPlayer.y = 0;
	//else
	//	directionToPlayer.y = directionToPlayer.y / std::abs(directionToPlayer.y) * 1.0f;

	return directionToPlayer;
}

void EnemyController::FixedUpdate(float deltaTime)
{
	if (m_pCharacterController == nullptr) return;

	Vector2<float> toPlayerVector{ GetDirectionToNearestPlayer() };
	if (toPlayerVector.x > 0){	
		toPlayerVector.x = std::clamp(toPlayerVector.x - CELL_WIDTH / 2.f, 0.f, 10000.0f);
	}
	else {
		toPlayerVector.x = std::clamp(toPlayerVector.x + CELL_WIDTH / 2.f, -10000.0f, 0.f);
	}
	if (toPlayerVector.y > 0) {
		toPlayerVector.y = std::clamp(toPlayerVector.y - CELL_WIDTH / 2.f, 0.f, 10000.0f);
	}
	else {
		toPlayerVector.y = std::clamp(toPlayerVector.y + CELL_WIDTH / 2.f, -10000.0f, 0.f);
	}


	if (!m_HasInitializedDirection) {
		//m_CurrentDirection = directionToPlayer;
		RecalculateDirection(toPlayerVector);
		m_HasInitializedDirection = true;
	}

	if (IsOnLadderChanged()
		|| IsOnPlatformChanged()) {
		RecalculateDirection(toPlayerVector);
	}
	else if (m_CanRandomlyChangeDirection)		
	{
		m_CurrentRandomDirectionChangeCooldown -= deltaTime;
		if (m_CurrentRandomDirectionChangeCooldown <= 0)
		{
			m_CurrentRandomDirectionChangeCooldown 
				= SimonGlobalFunctions::GetRandomFloat(m_MIN_RANDOM_DIRECTION_CHANGE_TIME, m_MAX_RANDOM_DIRECTION_CHANGE_TIME);

			RecalculateDirection(toPlayerVector);
		}
	
	}



	MoveCharacterInDirection();

	UpdateWasBooleans();


	//CalculateNewCanChangeVerticalDirection();
	//CalculateNewCanChangeHorizontalDirection();


	//if (m_CanRandomlyChangeDirection)
	//{
	//	m_CurrentRandomDirectionChangeCooldown -= deltaTime;
	//	if (m_CurrentRandomDirectionChangeCooldown <= 0)
	//	{
	//		m_CurrentRandomDirectionChangeCooldown 
	//			= SimonGlobalFunctions::GetRandomFloat(m_MIN_RANDOM_DIRECTION_CHANGE_TIME, m_MAX_RANDOM_DIRECTION_CHANGE_TIME);

	//		m_CanChangeHorizontalDirection = true;
	//		m_CanChangeVerticalDirection = true;
	//	}
	//}

	//TryChooseNewHorizontalDirection(directionToPlayer.x);
	//TryChooseNewVerticalDirection(directionToPlayer.y);

	////if (m_CurrentDirection.x < m_CurrentDirection.y)
	////	m_CurrentDirection.x = 0;
	////else if (m_CurrentDirection.y < m_CurrentDirection.x)
	////	m_CurrentDirection.y = 0;

	////
	//m_pCharacterController->MoveCharacter(m_CurrentDirection);
	////



	//m_WasOnLadder = *m_pIsOnLadder;
	//m_WasOnPlatform = *m_pIsOnPlatform || *m_pIsOnLeftPlatform || *m_pIsOnRightPlatform;
}

//void EnemyController::TryChooseNewHorizontalDirection(float toPlayerDirection)
//{
//	if (!m_CanChangeHorizontalDirection) return;
//	if (!*m_pIsOnPlatform) return;
//
//	if (*m_pIsOnPlatform) {
//		m_CurrentDirection.x = toPlayerDirection;// / std::abs(toPlayerDirection);
//		m_CanChangeVerticalDirection = false;
//	}
//
//	else if (toPlayerDirection > 0)
//	{
//		if (!*m_pIsOnRightPlatform)
//			return;
//		else{
//			m_CurrentDirection.x = toPlayerDirection; //1
//			m_CanChangeVerticalDirection = false;
//		}
//	}
//
//	else if (toPlayerDirection < 0)
//	{
//		if (!*m_pIsOnLeftPlatform)
//			return;
//		else{
//			m_CurrentDirection.x = toPlayerDirection;// -1;
//			m_CanChangeVerticalDirection = false;
//		}
//	}
//
//	m_CanChangeHorizontalDirection = false;
//}
//void EnemyController::TryChooseNewVerticalDirection(float toPlayerDirection)
//{
//	if (!m_CanChangeVerticalDirection) return;
//	if (!*m_pIsOnLadder) return;
//
//	if (toPlayerDirection > 0)
//	{
//		if (!*m_pIsOnUpwardsLadder)
//			return;
//		else {
//			m_CurrentDirection.y = toPlayerDirection;// 1;
//		}
//	}
//
//	else if (toPlayerDirection < 0)
//	{
//		if (!*m_pIsOnDownwardsLadder)
//			return;
//		else {
//			m_CurrentDirection.y = toPlayerDirection;// -1;
//		}
//	}
//
//
//	m_CanChangeVerticalDirection = false;
//}

//void EnemyController::CalculateNewCanChangeHorizontalDirection()
//{
//	if (m_WasOnPlatform != (*m_pIsOnPlatform || *m_pIsOnLeftPlatform || *m_pIsOnRightPlatform))
//	{
//		m_CanChangeHorizontalDirection = true;
//	}
//}
//void EnemyController::CalculateNewCanChangeVerticalDirection()
//{
//	if (m_WasOnLadder  != *m_pIsOnLadder)
//		m_CanChangeVerticalDirection = true;
//}

void EnemyController::RecalculateDirection(const Vector2<float> toPlayerVector)
{
	const bool isOnLadder			{ *m_pIsOnLadder };
	const bool isOnUpwardsLadder	{ *m_pIsOnUpwardsLadder };
	const bool isOnDownwardsLadder	{ *m_pIsOnDownwardsLadder };
	const bool isOnPlatform			{ *m_pIsOnPlatform };
	const bool isOnLeftPlatform		{ *m_pIsOnLeftPlatform };
	const bool isOnRightPlatform	{ *m_pIsOnRightPlatform };
	
	bool ignoreX{ false };
	// platform in both directions does not set other platform types' bools to true
	if (toPlayerVector.x > EPSILON
		&& !isOnPlatform
		&& !isOnRightPlatform)
		ignoreX = true;
	if (toPlayerVector.x < -EPSILON
		&& !isOnPlatform
		&& !isOnLeftPlatform)
		ignoreX = true;
	if (std::abs(toPlayerVector.x) < EPSILON)
		ignoreX = true;

	bool ignoreY{ false };
	// ladder in both directions also sets bools for other ladder types to true
	if (!isOnLadder)
		ignoreY = true;
	if (toPlayerVector.y > EPSILON
		&& !isOnUpwardsLadder)
		ignoreY = true;
	if (toPlayerVector.y < -EPSILON
		&& !isOnDownwardsLadder)
		ignoreY = true;
	if (std::abs(toPlayerVector.y) < EPSILON)
		ignoreY = true;

	if (!ignoreX
		&& !ignoreY)
	{
		if (toPlayerVector.x > toPlayerVector.y)
		{
			ignoreY = true;
		}
		else
			ignoreX = true;
	}

	if (ignoreY)
	{
		if (toPlayerVector.x > 0)
			m_CurrentNewDirection = Direction::right;
		else
			m_CurrentNewDirection = Direction::left;
	}
	else if (ignoreX)
	{
		if (toPlayerVector.y > 0)
			m_CurrentNewDirection = Direction::up;
		else
			m_CurrentNewDirection = Direction::down;
	}
	else
		std::cout << "Error while calculating EnemyController direction" << std::endl;
}

bool EnemyController::IsOnLadderChanged() const
{
	const bool isOnLadder{ *m_pIsOnLadder };

	if (isOnLadder != m_WasOnLadder)
		return true;
	return false;
}

bool EnemyController::IsOnPlatformChanged() const
{
	const bool isOnPlatform{ *m_pIsOnPlatform };
	const bool isOnLeftPlatform{ *m_pIsOnLeftPlatform };
	const bool isOnRightPlatform{ *m_pIsOnRightPlatform };

	if (isOnPlatform != m_WasOnPlatform)
		return true;
	if (isOnLeftPlatform != m_WasOnLeftPlatform)
		return true;
	if (isOnRightPlatform != m_WasOnRightPlatform)
		return true;
	return false;
}

void EnemyController::UpdateWasBooleans()
{
	m_WasOnLadder = (*m_pIsOnLadder);
	m_WasOnPlatform = (*m_pIsOnPlatform);
	m_WasOnLeftPlatform = (*m_pIsOnLeftPlatform);
	m_WasOnRightPlatform = (*m_pIsOnRightPlatform);
}

void EnemyController::MoveCharacterInDirection() const
{
	switch (m_CurrentNewDirection)
	{
	case Direction::left:
		m_pCharacterController->MoveCharacter({ -1,0 });
		break;
	case Direction::right:
		m_pCharacterController->MoveCharacter({ 1,0 });
		break;
	case Direction::up:
		m_pCharacterController->MoveCharacter({ 0,1 });
		break;
	case Direction::down:
		m_pCharacterController->MoveCharacter({ 0,-1 });
		break;
	default:
		std::cout << " EnemyController::MoveCharacterInDirection : direction not recognized" << std::endl;
		break;
	}
}
