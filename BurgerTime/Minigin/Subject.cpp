#include "Subject.h"


void Subject::AddObserver(Observer* pObserver)
{
	m_ObserverPtrs.push_back(pObserver);
}

void Subject::RemoveObserver(Observer* pObserver)
{
	std::vector<Observer*>::iterator pObserverPosIterator{ std::find(m_ObserverPtrs.begin(), m_ObserverPtrs.end(), pObserver) };
	if (pObserverPosIterator == m_ObserverPtrs.end()) return;
	m_ObserverPtrs.erase(pObserverPosIterator);
}

void Subject::NotifyObservers(Observer::Event event, void* pData)
{
	for(Observer * pObserver : m_ObserverPtrs)
	{
		pObserver->OnNotify(event, pData);
	}
}