#pragma once

#include <vector>
#include <string>

namespace FMOD::Studio
{
	class System;
	class EventInstance;
}

struct SoundHandle
{
public:
	SoundHandle() : m_Instance(nullptr) {}
	SoundHandle(FMOD::Studio::EventInstance* instance)
		: m_Instance(instance)
	{}

	void Stop();
	void Pause(bool pause);
	void SetVolume(float volume);
	bool IsPlaying() const;

private:
	FMOD::Studio::EventInstance* m_Instance;
};

class AudioManager
{
public:
	static bool Init();
	static void Update();
	static void Shutdown();
	static void PlayBackgroundMusic();
	static void PlayPreviousMusic();
	static void PlayBirds();
	static bool m_FadingOut;
	static bool m_FadingIn;
	static float m_CurrentVolume;
	static float m_TargetVolume;
	static float m_FadeDuration;
	static float m_FadeTimer;

	// TODO: Fade out and in background music, keep records of previous music?
	// PlaySound(string name)
	// return a handle to the sound, so you can pause it, set volume.
	static SoundHandle PlaySound(const char* soundName);

private:
	static FMOD::Studio::System* m_FMODSystem;
	static FMOD::Studio::EventInstance* m_BackgroundMusicInstance;

	static std::vector<std::string> m_BackgroundMusicPaths;
	static std::vector<int> m_TrackHistory;

	static int m_PendingNextTrackIndex;
	static bool m_PendingPlayPrevious;

	static bool LoadBank(const std::string& bankName);
	static void StopCurrentMusic();
	static void PlayTrack(int trackIndex, bool fadeIn);
	static void StartFadeOut();
	static void StartFadeIn();
};