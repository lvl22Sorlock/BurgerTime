//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "SoundSystemSDL.h"
#include "SDL_mixer.h"
#include <thread>

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
SoundSystemSDL::SoundSystemSDL()
	:SoundSystem()
	, m_SoundQueue{}
	, m_SoundNameHasher{}
	, m_SoundIds{}
	, m_Sounds{}
	, m_DEFAULT_VOLUME{64}
{}

SoundSystemSDL::~SoundSystemSDL()
{
	FreeMusic();
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void SoundSystemSDL::PlaySound(size_t soundId, int volume)
{
	m_SoundQueue.push(SoundPlayDescription(soundId, volume)); 
}

void SoundSystemSDL::PlaySound(size_t soundId)
{
	m_SoundQueue.push(SoundPlayDescription(soundId, m_DEFAULT_VOLUME));
}

void SoundSystemSDL::PlaySound(const std::string& soundName, int volume)
{
	m_SoundQueue.push(SoundPlayDescription(GetSoundId(soundName), volume));
}

void SoundSystemSDL::PlaySound(const std::string& soundName)
{
	m_SoundQueue.push(SoundPlayDescription(GetSoundId(soundName), m_DEFAULT_VOLUME));
}

void SoundSystemSDL::FreeMusic()
{
	for (std::pair<const size_t, Mix_Music*>& element : m_Sounds)
	{
		Mix_FreeMusic(element.second);
	}
}

bool SoundSystemSDL::IsSoundLoaded(size_t soundId) const
{
	return (m_Sounds.find(soundId) != m_Sounds.end());
}

void SoundSystemSDL::Update()
{
	if (m_SoundQueue.size() <= 0) return;

	if (!(Mix_PlayingMusic() || Mix_PausedMusic()))
	{
		std::jthread soundQueueThread{&SoundSystemSDL::ProcessSoundQueue, this};
		soundQueueThread.join();
	}
}

void SoundSystemSDL::ProcessSoundQueue()
{
	while (!m_SoundQueue.empty())
	{
		SoundPlayDescription soundDesc{ m_SoundQueue.front() };
		ProcessSound(soundDesc);
		m_SoundQueue.pop();
	}
}

void SoundSystemSDL::ProcessSound(const SoundPlayDescription& soundDescription)
{
	if (!IsSoundLoaded(soundDescription.soundId))
		LoadSound(soundDescription.soundId);
	if (Mix_PlayMusic(m_Sounds[soundDescription.soundId], 0) != -1)
	{	// succeeded
		if (soundDescription.isDifferentVolume)
		{
			Mix_VolumeMusic(soundDescription.soundVolume);
		}
	}
}

size_t SoundSystemSDL::GetSoundId(const std::string& name) const
{
	auto idIterator{ m_SoundIds.find(name) };
	if (idIterator == m_SoundIds.end())
	{
		std::cout << "Sound with name " << name << " not found" << std::endl;
		return 0;
	}

	return (*idIterator).second;
}

void SoundSystemSDL::AddSound(const std::string& soundName, const std::string& filePath)
{
	size_t soundId{ m_SoundNameHasher(soundName) };
	m_SoundIds[soundName] = soundId;
	m_SoundPaths[soundId] = filePath;
}

void SoundSystemSDL::LoadSound(size_t soundId)
{
	if (m_SoundPaths.find(soundId) == m_SoundPaths.end())
	{
		std::cout << "Trying to load sound but sound has no path" << std::endl;
		return;
	}

	Mix_Music* pNewSound{Mix_LoadMUS(m_SoundPaths[soundId].c_str())};
	if (!pNewSound)
	{
		std::cout << "Unable to load sound with id " << soundId << " and filePath " << m_SoundPaths[soundId] << std::endl;
		return;
	}
	m_Sounds[soundId] = pNewSound;
}
