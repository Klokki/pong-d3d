#pragma once

#include "pch.h"

struct SoundEvent
{
	IXAudio2SourceVoice* sourceVoice = nullptr;
	WAVEFORMATEX waveFormat = { 0 };
	unsigned int waveLength = NULL;
	std::vector<BYTE> audioData = { NULL };
	XAUDIO2_BUFFER audioBuffer = { 0 };
	unsigned int index = 0;
};