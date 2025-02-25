#include "siboxpch.h"
#include "Audio/AudioManager.h"

#include <fmod_studio.hpp>
#include <fmod_errors.h>

#include "Core/Application.h"

#include <random>

FMOD::Studio::System* AudioManager::m_FMODSystem = nullptr;
FMOD::Studio::EventInstance* AudioManager::m_BackgroundMusicInstance = nullptr;

std::vector<std::string> AudioManager::m_BackgroundMusicPaths = {
	"event:/Music/BackgroundMusic1",
	"event:/Music/BackgroundMusic2",
	"event:/Music/BackgroundMusic3"
};

std::vector<int> AudioManager::m_TrackHistory = {};

bool AudioManager::m_FadingOut = false;
bool AudioManager::m_FadingIn = false;
float AudioManager::m_CurrentVolume = 1.0f;
float AudioManager::m_TargetVolume = 1.0f; 
float AudioManager::m_FadeDuration = 1.0f;
float AudioManager::m_FadeTimer = 0.0f;

int AudioManager::m_PendingNextTrackIndex = -1;
bool AudioManager::m_PendingPlayPrevious = false;

bool AudioManager::Init()
{
	if (!Application::Get()->HasFrontend())
		return false;

	SIBOX_TRACE("Initialising AudioManager and FMOD.");
	Stopwatch sw;

	FMOD_RESULT result = FMOD::Studio::System::create(&m_FMODSystem);
	if (result != FMOD_OK)
	{
		std::string error = fmt::format("Failed to create FMOD system! Error: {}", FMOD_ErrorString(result));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return false;
	}

	FMOD_RESULT initResult = m_FMODSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
	if (initResult != FMOD_OK)
	{
		std::string error = fmt::format("Failed to initialise FMOD system! Error: {}", FMOD_ErrorString(initResult));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error!");
		return false;
	}

	if (!LoadBank("Master.bank"))
	{
		return false;
	}

	if (!LoadBank("Master.strings.bank"))
	{
		return false;
	}

	int newIndex = rand() % m_BackgroundMusicPaths.size();
	m_TrackHistory.push_back(newIndex);
	PlayTrack(newIndex, true);

	sw.End();
	SIBOX_TRACE("Initialised AudioManager/FMOD in {0}ms", sw.GetElapsedMilliseconds());
	return true;
}

bool AudioManager::LoadBank(const std::string& bankName)
{
	SIBOX_ASSERT(m_FMODSystem && "Attempting to load bank from uninitialised FMOD system");

	FMOD::Studio::Bank* bank;
	FMOD_RESULT result = m_FMODSystem->loadBankFile(fmt::format("Content/Audio/{}", bankName).c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	if (result != FMOD_OK)
	{
		std::string error = fmt::format("Failed to load bank file {}! Error: {}", bankName, FMOD_ErrorString(result));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return false;
	}
	return true;
}

void AudioManager::PlayBackgroundMusic()
{
	int lastIndex = m_TrackHistory.empty() ? -1 : m_TrackHistory.back();
	int newIndex;

	do {
		newIndex = rand() % m_BackgroundMusicPaths.size();
	} while (newIndex == lastIndex && m_BackgroundMusicPaths.size() > 1);

	m_PendingNextTrackIndex = newIndex;
	m_PendingPlayPrevious = false;

	if (m_BackgroundMusicInstance)
	{
		StartFadeOut();
	}
	else
	{
		m_TrackHistory.push_back(newIndex);
		PlayTrack(newIndex, true);
	}
}

void AudioManager::PlayPreviousMusic()
{
	if (m_TrackHistory.size() < 2)
	{
		SIBOX_WARN("No previous background music instance to play!");
		return;
	}

	m_PendingPlayPrevious = true;
	m_PendingNextTrackIndex = -1;

	if (m_BackgroundMusicInstance)
	{
		StartFadeOut();
	}
	else
	{
		m_TrackHistory.pop_back();
		int previousIndex = m_TrackHistory.back();
		PlayTrack(previousIndex, true);
	}
}

void AudioManager::PlayTrack(int trackIndex, bool fadeIn)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	FMOD_RESULT result = m_FMODSystem->getEvent(m_BackgroundMusicPaths[trackIndex].c_str(), &eventDescription);
	if (result != FMOD_OK)
		{
			std::string error = fmt::format("Failed to get background music event! Error: {}", FMOD_ErrorString(result));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return;
		}
	
	result = eventDescription->createInstance(&m_BackgroundMusicInstance);
	if (result != FMOD_OK)
	{
		std::string error = fmt::format("Failed to create background music instance! Error: {}", FMOD_ErrorString(result));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return;
	}

	if (fadeIn)
	{
		m_BackgroundMusicInstance->setVolume(0.0f);
		StartFadeIn();
	}
	else
	{
		m_BackgroundMusicInstance->setVolume(1.0f);
		m_CurrentVolume = 1.0f;
	}

	result = m_BackgroundMusicInstance->start();
	if (result != FMOD_OK)
	{
		std::string error = fmt::format("Failed to start background music instance! Error: {}", FMOD_ErrorString(result));
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return;
	}

	SIBOX_INFO("Playing track index: {} ({})", trackIndex, m_BackgroundMusicPaths[trackIndex]);
    SIBOX_INFO("Current History: ");
    for (auto idx : m_TrackHistory) {
        SIBOX_INFO("   {}", m_BackgroundMusicPaths[idx]);
    }

	SIBOX_INFO("Playing track index: {} ({})", trackIndex, m_BackgroundMusicPaths[trackIndex]);
	SIBOX_INFO("Current History: ");
	for (auto idx : m_TrackHistory) {
		SIBOX_INFO("   {}", m_BackgroundMusicPaths[idx]);
	}
}

void AudioManager::StopCurrentMusic()
{
	if (m_BackgroundMusicInstance)
	{
		m_BackgroundMusicInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		m_BackgroundMusicInstance->release();
		m_BackgroundMusicInstance = nullptr;
		m_CurrentVolume = 1.0f;
	}
}

void AudioManager::StartFadeOut()
{
	m_FadingOut = true;
	m_FadingIn = false;
	m_TargetVolume = 0.0f;
	m_FadeTimer = 0.0f;
}

void AudioManager::StartFadeIn()
{
	m_FadingIn = true;
	m_FadingOut = false;
	m_TargetVolume = 1.0f;
	m_FadeTimer = 0.0f;
	m_CurrentVolume = 0.0f;
}

void AudioManager::PlayBirds()
{
	static const float minInterval = 10.0f;
	static const float maxInterval = 100.0f;

	static float timer = 0.0f;
	static float nextInterval = minInterval + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxInterval - minInterval)));
	static SoundHandle birdSound;

	float deltaTime = Application::Get()->GetDeltaTime();
	timer += deltaTime;

	if (birdSound.IsPlaying())
	{
		return;
	}


	if (timer >= nextInterval)
	{
		timer = 0.0f;
		nextInterval = minInterval + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxInterval - minInterval)));

		birdSound = PlaySound("event:/SFX/Birds");

		static const float minVolume = 0.5f;
		static const float maxVolume = 3.0f;

		float randomVolume = minVolume + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVolume - minVolume)));
		birdSound.SetVolume(randomVolume);
	}
}

SoundHandle AudioManager::PlaySound(const char* soundName)
{
	if (!m_FMODSystem)
	{
		SIBOX_WARN("FMOD system not initialized, cannot play sound!");
		return SoundHandle(nullptr);
	}

	FMOD::Studio::EventDescription* eventDescription = nullptr;
	FMOD_RESULT result = m_FMODSystem->getEvent(soundName, &eventDescription);
	if (result != FMOD_OK || !eventDescription)
	{
		std::string error = fmt::format("Failed to get event for sound: {}", soundName);
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return SoundHandle(nullptr);
	}

	FMOD::Studio::EventInstance* instance = nullptr;
	result = eventDescription->createInstance(&instance);
	if (result != FMOD_OK || !instance)
	{
		std::string error = fmt::format("Failed to create instance for sound: {}", soundName);
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		return SoundHandle(nullptr);
	}

	result = instance->start();
	if (result != FMOD_OK)
	{
		std::string error = fmt::format("Failed to start sound: {}", soundName);
		SIBOX_ERROR("{}", error);
		Application::Get()->ShowError(error.c_str(), "FMOD Error");
		instance->release();
		return SoundHandle(nullptr);
	}

	return SoundHandle(instance);
}

void SoundHandle::Stop()
{
	if (m_Instance)
	{
		m_Instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		m_Instance->release();
		m_Instance = nullptr;
	}
}

void SoundHandle::Pause(bool pause)
{
	if (m_Instance)
	{
		m_Instance->setPaused(pause);
	}
}

void SoundHandle::SetVolume(float volume)
{
	if (m_Instance)
	{
		m_Instance->setVolume(volume);
	}
}

bool SoundHandle::IsPlaying() const
{
	if (!m_Instance)
	{
		return false;
	}

	FMOD_STUDIO_PLAYBACK_STATE playbackState;
	FMOD_RESULT result = m_Instance->getPlaybackState(&playbackState);

	if (result != FMOD_OK)
	{
		SIBOX_ERROR("Failed to retrieve playback state: {}", FMOD_ErrorString(result));
		return false;
	}

	return playbackState == FMOD_STUDIO_PLAYBACK_PLAYING;
}

void AudioManager::Update()
{
	if (!Application::Get()->HasFrontend())
		return;

	float deltaTime = Application::Get()->GetDeltaTime();

	if (m_FadingOut && m_BackgroundMusicInstance)
	{
		m_FadeTimer += deltaTime;
		float progress = m_FadeTimer / m_FadeDuration;
		if (progress >= 1.0f)
		{
			progress = 1.0f;
			m_FadingOut = false;
			m_CurrentVolume = 0.0f;
			m_BackgroundMusicInstance->setVolume(m_CurrentVolume);
			StopCurrentMusic();

			if (m_PendingPlayPrevious)
			{
				m_PendingPlayPrevious = false;
				m_TrackHistory.pop_back();
				int previousIndex = m_TrackHistory.back();
				PlayTrack(previousIndex, true);
			}
			else if (m_PendingNextTrackIndex != -1)
			{
				m_TrackHistory.push_back(m_PendingNextTrackIndex);
				int newIndex = m_PendingNextTrackIndex;
				m_PendingNextTrackIndex = -1;
				PlayTrack(newIndex, true);
			}
		}
		else
		{
			m_CurrentVolume = 1.0f - progress;
			m_BackgroundMusicInstance->setVolume(m_CurrentVolume);
		}
	}

	if (m_FadingIn && m_BackgroundMusicInstance)
	{
		m_FadeTimer += deltaTime;
		float progress = m_FadeTimer / m_FadeDuration;
		if (progress >= 1.0f)
		{
			progress = 1.0f;
			m_FadingIn = false;
		}
		m_CurrentVolume = progress;
		m_BackgroundMusicInstance->setVolume(m_CurrentVolume);
	}

	if (m_FMODSystem)
	{
		FMOD_RESULT result = m_FMODSystem->update();
		if (result != FMOD_OK)
		{
			std::string error = fmt::format("Failed to update FMOD system! Error: {}", FMOD_ErrorString(result));
			SIBOX_ERROR("{}", error);
			Application::Get()->ShowError(error.c_str(), "FMOD Error");
		}
	}

	PlayBirds();
}

void AudioManager::Shutdown()
{
	if (!Application::Get()->HasFrontend())
		return;

	SIBOX_TRACE("Shutting down AudioManager and FMOD.");

	if (m_BackgroundMusicInstance)
	{
		m_BackgroundMusicInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		m_BackgroundMusicInstance->release();
		m_BackgroundMusicInstance = nullptr;
	}

	if (m_FMODSystem)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}