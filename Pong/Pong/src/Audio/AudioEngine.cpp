#include "pch.h"
#include "AudioEngine.hpp"

AudioEngine::AudioEngine()
{
	HRESULT hr;

	// create XAudio2 device and mastering voice
	if (FAILED(hr = XAudio2Create(m_audioDevice.GetAddressOf())))
		Error::Message(hr, "Failed to create XAUDIO2 device");

	if (FAILED(hr = m_audioDevice->CreateMasteringVoice(&m_masteringVoice)))
		Error::Message(hr, "Failed to create XAUDIO2 mastering voice");
}