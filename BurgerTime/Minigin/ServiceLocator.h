#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
class SoundSystem;

//-------------------------------------------------------------------------
//	ServiceLocator Class
//-------------------------------------------------------------------------
class ServiceLocator final : public dae::Singleton<ServiceLocator>
{
public:
	~ServiceLocator() = default;
	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	ServiceLocator(const ServiceLocator& other)						= delete;
	ServiceLocator(ServiceLocator&& other) noexcept					= delete;
	ServiceLocator& operator=(const ServiceLocator& other)			= delete;
	ServiceLocator& operator=(ServiceLocator&& other)	noexcept	= delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	SoundSystem* GetSoundSystem();
	void RegisterSoundSystem(SoundSystem* pSoundSystem);

private:
	friend class Singleton<ServiceLocator>;
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	ServiceLocator();
	SoundSystem* m_pSoundSystem;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
};
