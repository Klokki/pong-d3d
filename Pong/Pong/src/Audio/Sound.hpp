#pragma once

#include "pch.h"

struct Sound
{
    IXAudio2SourceVoice* sourceVoice = nullptr;
    WAVEFORMATEX waveFormat = {};
    unsigned int waveLength = NULL;
    std::vector<BYTE> audioData;
    XAUDIO2_BUFFER audioBuffer = {};
};
