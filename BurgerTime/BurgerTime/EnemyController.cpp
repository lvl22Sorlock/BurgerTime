//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "BurgerTimePCH.h"
#include "EnemyController.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "ComponentCharacterController.h"
#include "BurgerTimeGlobal.h"

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

	Vector2<float> directionToPlayer{ GetDirectionToNearestPlayer() };
	if (!m_HasInitializedDirection) {
		m_CurrentDirection = directionToPlayer;
		m_HasInitializedDirection = true;
	}

	CalculateNewCanChangeVerticalDirection();
	CalculateNewCanChangeHorizontalDirection();


	if (m_CanRandomlyChangeDirection)
	{
		m_CurrentRandomDirectionChangeCooldown -= deltaTime;
		if (m_CurrentRandomDirectionChangeCooldown <= 0)
		{
			m_CurrentRandomDirectionChangeCooldown 
				= SimonGlobalFunctions::GetRandomFloat(m_MIN_RANDOM_DIRECTION_CHANGE_TIME, m_MAX_RANDOM_DIRECTION_CHANGE_TIME);

			m_CanChangeHorizontalDirection = true;
			m_CanChangeVerticalDirection = true;
		}
	}

	TryChooseNewHorizontalDirection(directionToPlayer.x);
	TryChooseNewVerticalDirection(directionToPlayer.y);

	//if (m_CurrentDirection.x < m_CurrentDirection.y)
	//	m_CurrentDirection.x = 0;
	//else if (m_CurrentDirection.y < m_CurrentDirection.x)
	//	m_CurrentDirection.y = 0;

	//
	m_pCharacterController->MoveCharacter(m_CurrentDirection);
	//



	m_WasOnLadder = *m_pIsOnLadder;
	m_WasOnPlatform = *m_pIsOnPlatform || *m_pIsOnLeftPlatform || *m_pIsOnRightPlatform;
}

void EnemyController::TryChooseNewHorizontalDirection(float toPlayerDirection)
{
	if (!m_CanChangeHorizontalDirection) return;
	if (!*m_pIsOnPlatform) return;

	if (*m_pIsOnPlatform) {
		m_CurrentDirection.x = toPlayerDirection;// / std::abs(toPlayerDirection);
		m_CanChangeVerticalDirection = false;
	}

	else if (toPlayerDirection > 0)
	{
		if (!*m_pIsOnRightPlatform)
			return;
		else{
			m_CurrentDirection.x = toPlayerDirection; //1
			m_CanChangeVerticalDirection = false;
		}
	}

	else if (toPlayerDirection < 0)
	{
		if (!*m_pIsOnLeftPlatform)
			return;
		else{
			m_CurrentDirection.x = toPlayerDirection;// -1;
			m_CanChangeVerticalDirection = false;
		}
	}

	m_CanChangeHorizontalDirection = false;
}
void EnemyController::TryChooseNewVerticalDirection(float toPlayerDirection)
{
	if (!m_CanChangeVerticalDirection) return;
	if (!*m_pIsOnLadder) return;

	if (toPlayerDirection > 0)
	{
		if (!*m_pIsOnUpwardsLadder)
			return;
		else {
			m_CurrentDirection.y = toPlayerDirection;// 1;
		}
	}

	else if (toPlayerDirection < 0)
	{
		if (!*m_pIsOnDownwardsLadder)
			return;
		else {
			m_CurrentDirection.y = toPlayerDirection;// -1;
		}
	}


	m_CanChangeVerticalDirection = false;
}

void EnemyController::CalculateNewCanChangeHorizontalDirection()
{
	if (m_WasOnPlatform != (*m_pIsOnPlatform || *m_pIsOnLeftPlatform || *m_pIsOnRightPlatform))
	//if (m_WasOnLadder != *m_pIsOnLadder)
	{
		m_CanChangeHorizontalDirection = true;
	}
}
void EnemyController::CalculateNewCanChangeVerticalDirection()
{
	if (m_WasOnLadder  != *m_pIsOnLadder)
	//if (m_WasOnPlatform != (*m_pIsOnPlatform || *m_pIsOnLeftPlatform || *m_pIsOnRightPlatform))
		m_CanChangeVerticalDirection = true;
}
