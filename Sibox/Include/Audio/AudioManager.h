#pragma once

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
private:
	static FMOD::Studio::System* m_FMODSystem;
};