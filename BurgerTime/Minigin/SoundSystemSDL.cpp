//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "SoundSystemSDL.h"


//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
SoundSystemSDL::SoundSystemSDL()
	:SoundSystem()
{

}

SoundSystemSDL::~SoundSystemSDL()
{
	FreeMusic();
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

//void SoundSystemSDL::PlaySound(int soundId, float volume)
//{
//	if (!IsSoundLoaded(soundId))
//	{
//		std::cout << "Trying to play soundId but no associated sound was found" << std::endl;
//		return;
//	}
//
//
//}
//
//void SoundSystemSDL::PlaySound(int soundId)
//{
//	if (!IsSoundLoaded(soundId))
//	{
//		std::cout << "Trying to play soundId but no associated sound was found" << std::endl;
//		return;
//	}
//
//
//}

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
		SoundPlayDescription soundDesc{m_SoundQueue.front()};
		if (Mix_PlayMusic(m_Sounds[soundDesc.soundId], 0) != -1)
		{	// succeeded
			if (soundDesc.isDifferentVolume)			
				{Mix_VolumeMusic(soundDesc.soundVolume); }
			
		}
		m_SoundQueue.pop();
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
	
void SoundSystemSDL::LoadSound(const std::string& soundName, const std::string& filePath)
{
	if (m_SoundIds.find(soundName) != m_SoundIds.end())
	{
		std::cout << "Sound with name " << soundName << " was already loaded" << std::endl;
 		return;
	}

	Mix_Music* pNewSound{Mix_LoadMUS(filePath.c_str())};
	if (!pNewSound)
	{
		std::cout << "Unable to load sound with name " << soundName << " and filePath " << filePath << std::endl;
		return;
	}

	m_SoundIds[soundName] = m_SoundNameHasher(soundName);
	m_Sounds[m_SoundIds[soundName]] = pNewSound;
}
