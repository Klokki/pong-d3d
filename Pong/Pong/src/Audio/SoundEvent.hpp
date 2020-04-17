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
	WAVEFORMATEX waveFormat;
	unsigned int waveLength = NULL;
	std::vector<BYTE> audioData = { NULL };
	XAUDIO2_BUFFER audioBuffer;
	unsigned int index = 0;
};