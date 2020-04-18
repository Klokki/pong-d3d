#pragma once

#include "AudioEngine.hpp"
#include "Sound.hpp"

class AudioComponent
{
public:
	AudioComponent();
	~AudioComponent();

	void LoadFile(const std::wstring fileName, Sound& soundEvent);
	void PlaySound(const Sound& soundEvent);
	void StopSound(const Sound& soundEvent);
private:
	AudioEngine* m_audioEngine;
};