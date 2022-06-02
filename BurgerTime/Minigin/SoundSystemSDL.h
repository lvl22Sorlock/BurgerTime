#pragma once

//https://www.zophar.net/music/gameboy-gbs/burgertime-deluxe

//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "SDL_mixer.h"
#include "SoundSystem.h"
//#include "Singleton.h"
#include <unordered_map>

//using namespace dae;

//-------------------------------------------------------------------------
//	SoundSystemSDL Class
//-------------------------------------------------------------------------
class SoundSystemSDL final : public SoundSystem
{
public:
	SoundSystemSDL();
	~SoundSystemSDL();

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	SoundSystemSDL(const SoundSystemSDL& other) = delete;
	SoundSystemSDL(SoundSystemSDL&& other) noexcept = delete;
	SoundSystemSDL& operator=(const SoundSystemSDL& other) = delete;
	SoundSystemSDL& operator=(SoundSystemSDL&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual void Update() override;
	virtual size_t GetSoundId(const std::string& name) const override;
	//virtual void PlaySound(int soundId, float volume) override;
	//virtual void PlaySound(int soundId) override;
	virtual void LoadSound(const std::string& soundName, const std::string& filePath) override;


private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------
	
	void FreeMusic();
	bool IsSoundLoaded(size_t soundId) const;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	std::unordered_map<std::string, size_t> m_SoundIds;
	std::unordered_map<size_t, Mix_Music*> m_Sounds;

private:

};


//// Code Snippets from demo
// 
//if (Mix_PausedMusic())
//Mix_ResumeMusic();
//else
//Mix_PauseMusic();

//volume = (volume + 1) << 1;
//if (volume > SDL_MIX_MAXVOLUME)
//volume = SDL_MIX_MAXVOLUME;
//Mix_VolumeMusic(volume);