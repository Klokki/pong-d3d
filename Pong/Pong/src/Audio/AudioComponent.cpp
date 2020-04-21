#include "pch.h"
#include "AudioComponent.hpp"

AudioComponent::AudioComponent()
    :
    m_audioEngine()
{
}

AudioComponent::~AudioComponent()
{
}

void AudioComponent::LoadFile(const std::wstring filename, Sound& sound)
{
    HRESULT hr;
    WAVEFORMATEX* waveFormat;

    m_audioEngine.loadFile(filename, sound.audioData, &waveFormat, sound.waveLength);
    sound.waveFormat = *waveFormat;

    if (FAILED(hr = m_audioEngine.m_audioDevice->CreateSourceVoice(&sound.sourceVoice, &sound.waveFormat)))
        Error::Message(hr, "Failed to create source voice");

    ZeroMemory(&sound.audioBuffer, sizeof(XAUDIO2_BUFFER));
    sound.audioBuffer.AudioBytes = (UINT32)sound.audioData.size();
    sound.audioBuffer.pAudioData = (BYTE* const)&sound.audioData[0];
    sound.audioBuffer.pContext = nullptr;
    sound.audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
}

void AudioComponent::PlaySound(const Sound& sound)
{
    HRESULT hr;

    // submit audio buffer to source voice
    if (FAILED(hr = sound.sourceVoice->SubmitSourceBuffer(&sound.audioBuffer)))
        Error::Message(hr, "Failed to submit source buffer");

    if (FAILED(hr = sound.sourceVoice->Start()))
        Error::Message(hr, "Failed to start Sound");
}

void AudioComponent::StopSound(const Sound& sound)
{
    HRESULT hr;

    if (FAILED(hr = sound.sourceVoice->Stop()))
        Error::Message(hr, "Failed to stop Sound");
}