#pragma once
#include <vector>
#include "Observer.h"

class Subject
{
public:
	Subject() = default;
	virtual ~Subject() = default;


	virtual void AddObserver(Observer* pObserver);
	virtual void RemoveObserver(Observer* pObserver);

	Subject(const Subject& other) = delete;
	Subject(Subject&& other) noexcept = delete;
	Subject& operator=(const Subject& other) = delete;
	Subject& operator=(Subject&& other)	noexcept = delete;
protected:
	virtual void NotifyObservers(Observer::Event event, void* pData = nullptr);

private:
	std::vector<Observer*> m_ObserverPtrs;
};
