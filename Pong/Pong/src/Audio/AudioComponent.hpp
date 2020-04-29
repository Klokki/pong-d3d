#pragma once

#include "AudioEngine.hpp"
#include "Sound.hpp"

class AudioComponent
{
public:
    AudioComponent();
    ~AudioComponent();

    void LoadFile(const std::wstring filename, const std::string name, Sound& sound);
    void PlaySound(const std::string name);
    void StopSound(const std::string name);
    void StopSounds();
private:
    AudioEngine m_audioEngine;
    std::map<std::string, Sound> m_sounds;
};
