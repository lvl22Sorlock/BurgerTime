#pragma once
#include <iostream>
#include "GameObject.h"

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