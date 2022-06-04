#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	SoundSystem Class
//-------------------------------------------------------------------------
class SoundSystem
{
public:
	SoundSystem() = default;
	virtual ~SoundSystem() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	SoundSystem(const SoundSystem& other) = delete;
	SoundSystem(SoundSystem&& other) noexcept = delete;
	SoundSystem& operator=(const SoundSystem& other) = delete;
	SoundSystem& operator=(SoundSystem&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Update() = 0;

	virtual size_t GetSoundId(const std::string& name) const = 0;
	virtual void PlaySound(size_t soundId, int volume) = 0;
	virtual void PlaySound(size_t soundId) = 0;
	virtual void PlaySound(const std::string& soundName, int volume) = 0;
	virtual void PlaySound(const std::string& soundName) = 0;
	virtual void AddSound(const std::string& soundName, const std::string& filePath) = 0;

protected:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

};