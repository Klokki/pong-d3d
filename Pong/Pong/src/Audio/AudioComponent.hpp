#pragma once

#include "AudioEngine.hpp"
#include "SoundEvent.hpp"

class AudioComponent
{
public:
	AudioComponent();
	~AudioComponent();

	void LoadFile(const std::wstring fileName, SoundEvent& soundEvent);
	void PlaySound(const SoundEvent& soundEvent);
private:
	AudioEngine* m_audioEngine;
};