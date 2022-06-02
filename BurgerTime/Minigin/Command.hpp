#pragma once
#include <iostream>
#include "GameObject.h"
#include "ComponentHealth.h"
#include "ComponentScoreManager.h"

class Command
{
public:
	Command() = default;
	virtual ~Command() = default;
	virtual void Execute() = 0;

	Command(const Command& other) = delete;
	Command(Command&& other) noexcept = delete;
	Command& operator=(const Command& other) = delete;
	Command& operator=(Command&& other)	noexcept = delete;
};

class JumpCommand : public Command
{
public:
	void Execute() override { std::cout << "Jump!" << std::endl; }
};
class FireCommand : public Command
{
public:
	void Execute() override { std::cout << "Oh no there's a fire!" << std::endl; }
};
class DuckCommand : public Command
{
public:
	void Execute() override { std::cout << "It's a goose, not a duck" << std::endl; }
};
class FartCommand : public Command
{
public:
	void Execute() override { std::cout << "2d8 poison damage, DC 18 CON save for half damage" << std::endl; }
};


class DieCommand : public Command
{
public:
	DieCommand(dae::GameObject* pPlayer)
		:Command()
		, m_pPlayer{pPlayer}
	{
	}
	void Execute() override {
		ComponentHealth* pComponentHealth{ m_pPlayer->GetComponentPtr<ComponentHealth>() };
		pComponentHealth->DealDamage(pComponentHealth->GetCurrentHealth());
	}

protected:
	dae::GameObject* m_pPlayer;
};

class GainScoreCommand : public Command
{
public:
	GainScoreCommand(dae::GameObject* pPlayer)
		:Command()
		, m_pPlayer{ pPlayer }
	{
	}
	void Execute() override {
		ComponentScoreManager* pComponentScoreManager{ m_pPlayer->GetComponentPtr<ComponentScoreManager>() };
		pComponentScoreManager->IncreaseScore(500);
	}

protected:
	dae::GameObject* m_pPlayer;
};