#pragma once

//https://www.zophar.net/music/gameboy-gbs/burgertime-deluxe

//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "SDL_mixer.h"
#include "SoundSystem.h"
#include <queue>
#include <string>
#include <unordered_map>

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
	virtual void AddSound(const std::string& soundName, const std::string& filePath) override;	
	void LoadSound(size_t soundId);


	virtual void PlaySound(size_t soundId, int volume) override;
	virtual void PlaySound(size_t soundId) override;
	virtual void PlaySound(const std::string& soundName, int volume) override;
	virtual void PlaySound(const std::string& soundName) override;

private:
	struct SoundPlayDescription
	{
		size_t soundId{};
		bool isDifferentVolume{};
		int soundVolume{};

		SoundPlayDescription(size_t id)
			:soundId{ id }
			, isDifferentVolume{ false }
			, soundVolume{ 64 }
		{}

		SoundPlayDescription(size_t id, int volume)
			:soundId{ id }
			, isDifferentVolume{ true }
			, soundVolume{ volume }
		{
			soundVolume = std::clamp(soundVolume, 0, SDL_MIX_MAXVOLUME);
		}
	};
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------
	
	void FreeMusic();
	bool IsSoundLoaded(size_t soundId) const;
	void ProcessSound(const SoundPlayDescription& soundDescription);
	void ProcessSoundQueue();

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	std::queue<SoundPlayDescription> m_SoundQueue;
	std::hash<std::string> m_SoundNameHasher;
	std::unordered_map<std::string, size_t> m_SoundIds;
	std::unordered_map<size_t, std::string> m_SoundPaths;
	std::unordered_map<size_t, Mix_Music*> m_Sounds;
	const int m_DEFAULT_VOLUME;
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