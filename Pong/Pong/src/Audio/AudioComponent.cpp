#include "pch.h"
#include "AudioComponent.hpp"

AudioComponent::AudioComponent()
{
	m_audioEngine = new AudioEngine();
}

AudioComponent::~AudioComponent()
{
	delete m_audioEngine;
}

void AudioComponent::LoadFile(const std::wstring filename, SoundEvent& soundEvent)
{
	HRESULT hr;
	WAVEFORMATEX* waveFormat;

	m_audioEngine->loadFile(filename, soundEvent.audioData, &waveFormat, soundEvent.waveLength);
	soundEvent.waveFormat = *waveFormat;

	if (FAILED(hr = m_audioEngine->m_audioDevice->CreateSourceVoice(&soundEvent.sourceVoice, &soundEvent.waveFormat)))
		Error::Message(hr, "Failed to create source voice");

	ZeroMemory(&soundEvent.audioBuffer, sizeof(XAUDIO2_BUFFER));
	soundEvent.audioBuffer.AudioBytes = (UINT32)soundEvent.audioData.size();
	soundEvent.audioBuffer.pAudioData = (BYTE* const)&soundEvent.audioData[0];
	soundEvent.audioBuffer.pContext = nullptr;
}

void AudioComponent::PlaySound(const SoundEvent& soundEvent)
{
	HRESULT hr;

	// submit audio buffer to source voice
	if (FAILED(hr = soundEvent.sourceVoice->SubmitSourceBuffer(&soundEvent.audioBuffer)))
		Error::Message(hr, "Failed to submit source buffer");

	if (FAILED(hr = soundEvent.sourceVoice->Start()))
		Error::Message(hr, "Failed to start SoundEvent");
}
