#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include <queue>
#include <string>

//-------------------------------------------------------------------------
//	SoundSystem Class
//-------------------------------------------------------------------------
class SoundSystem
{
public:
	SoundSystem();
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
	virtual void PlaySound(size_t soundId, int volume) { m_SoundQueue.push(SoundPlayDescription(soundId, volume)); };
	virtual void PlaySound(size_t soundId) { m_SoundQueue.push(SoundPlayDescription(soundId)); };
	virtual void LoadSound(const std::string& soundName, const std::string& filePath) = 0;

protected:
	struct SoundPlayDescription
	{
		size_t soundId{};
		bool isDifferentVolume{};
		int soundVolume{};		

		SoundPlayDescription(size_t id)
			:soundId{id}
			,isDifferentVolume{false}
			,soundVolume{64}
		{}

		SoundPlayDescription(size_t id, int volume)
			:soundId{ id }
			, isDifferentVolume{ true }
			, soundVolume{ volume }
		{
			if (soundVolume > SDL_MIX_MAXVOLUME)
			{
				soundVolume = SDL_MIX_MAXVOLUME;
			}
		}
	};

	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	std::queue<SoundPlayDescription> m_SoundQueue;
	std::hash<std::string> m_SoundNameHasher;


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

};