#include "pch.h"
#include "AudioComponent.hpp"

AudioComponent::AudioComponent()
	:
	m_audioEngine()
{
}

AudioComponent::~AudioComponent()
{
	StopSounds();
}

void AudioComponent::LoadFile(const std::wstring filename, const std::string name, Sound& sound)
{
	HRESULT hr;
	WAVEFORMATEX* waveFormat;

	m_audioEngine.loadFile(filename, sound.audioData, &waveFormat, sound.waveLength);
	sound.waveFormat = *waveFormat;

	if (FAILED(hr = m_audioEngine.m_audioDevice->CreateSourceVoice(&sound.sourceVoice, &sound.waveFormat)))
		Error::Message(hr, "Failed to create source voice");

	sound.audioBuffer.AudioBytes = (UINT32)sound.audioData.size();
	sound.audioBuffer.pAudioData = (BYTE* const)&sound.audioData[0];
	sound.audioBuffer.pContext = nullptr;
	sound.audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

	m_sounds[name] = sound;
}

void AudioComponent::PlaySound(const std::string name)
{
	HRESULT hr;

	// submit audio buffer to source voice
	if (FAILED(hr = m_sounds[name].sourceVoice->SubmitSourceBuffer(&m_sounds[name].audioBuffer)))
		Error::Message(hr, "Failed to submit source buffer");

	if (FAILED(hr = m_sounds[name].sourceVoice->Start()))
		Error::Message(hr, "Failed to start Sound");
}

void AudioComponent::StopSound(const std::string name)
{
	HRESULT hr;

	if (FAILED(hr = m_sounds[name].sourceVoice->Stop()))
		Error::Message(hr, "Failed to stop Sound" + name);
}

void AudioComponent::StopSounds()
{
	HRESULT hr;

	// iterate through the map of sounds and stop all of them
	for (auto const& [key, sound] : m_sounds)
	{
		if (FAILED(hr = sound.sourceVoice->Stop()))
			Error::Message(hr, "Failed to stop sound " + key);
	}
}
