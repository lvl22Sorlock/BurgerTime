#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
//class SoundSystem;
#include "SoundSystem.h"

using namespace dae;

//-------------------------------------------------------------------------
//	ServiceLocator Class
//-------------------------------------------------------------------------
class ServiceLocator final : public Singleton<ServiceLocator>
{
public:
	ServiceLocator();
	~ServiceLocator();

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	ServiceLocator(const ServiceLocator& other) = delete;
	ServiceLocator(ServiceLocator&& other) noexcept = delete;
	ServiceLocator& operator=(const ServiceLocator& other) = delete;
	ServiceLocator& operator=(ServiceLocator&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	SoundSystem& GetSoundSystem();
	void RegisterSoundSystem(SoundSystem* pSoundSystem);

protected:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	SoundSystem* m_pSoundSystem;


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------




};
