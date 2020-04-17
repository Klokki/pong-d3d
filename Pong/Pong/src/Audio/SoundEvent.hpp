#pragma once

#include "pch.h"

class SoundEvent
{
	friend class AudioComponent;
public:
	SoundEvent(){}
	~SoundEvent(){}
private:
	IXAudio2SourceVoice* sourceVoice = nullptr;
	WAVEFORMATEX waveFormat = { 0 };
	unsigned int waveLength = NULL;
	std::vector<BYTE> audioData = { NULL };
	XAUDIO2_BUFFER audioBuffer = { 0 };
	unsigned int index = 0;
};