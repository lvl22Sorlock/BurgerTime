//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
ServiceLocator::ServiceLocator()
	:m_pSoundSystem{nullptr}
{

}

ServiceLocator::~ServiceLocator()
{

}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

SoundSystem& ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystem;
}

void ServiceLocator::RegisterSoundSystem(SoundSystem* pSoundSystem)
{
	m_pSoundSystem = pSoundSystem;
}
