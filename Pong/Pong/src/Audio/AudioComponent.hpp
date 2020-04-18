#pragma once

#include "AudioEngine.hpp"
#include "Sound.hpp"

class AudioComponent
{
public:
	AudioComponent();
	~AudioComponent();

	void LoadFile(const std::wstring fileName, Sound& sound);
	void PlaySound(const Sound& sound);
	void StopSound(const Sound& sound);
private:
	AudioEngine m_audioEngine;
};