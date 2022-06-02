#pragma once
class Observer
{
public:
	enum class Event
	{
		playerDied,
		scoreGained,
		playerHealthChanged
	};

	Observer() = default;
	virtual ~Observer() = default;
	virtual void OnNotify(Event event, void* pData = nullptr) = 0;

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) noexcept = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other)	noexcept = delete;
};

