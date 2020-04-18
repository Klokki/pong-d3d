#pragma once

#include "pch.h"

struct SoundEvent
{
	IXAudio2SourceVoice* sourceVoice = nullptr;
	WAVEFORMATEX waveFormat = { 0 };
	unsigned int waveLength = NULL;
	std::vector<BYTE> audioData;
	XAUDIO2_BUFFER audioBuffer = { 0 };
};