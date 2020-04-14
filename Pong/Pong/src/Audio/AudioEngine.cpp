#include "pch.h"
#include "AudioEngine.hpp"

AudioEngine::AudioEngine()
{
	HRESULT hr;

	// start Windows Media Foundation
	if (FAILED(hr = MFStartup(MF_VERSION)))
		Error::Message(hr, "Failed to start Windows Media Foundation");

	if (FAILED(hr = MFCreateAttributes(m_sourceReaderConfiguration.GetAddressOf(), 1)))
		Error::Message(hr, "Failed to create source reader configuration");

	if (FAILED(hr = m_sourceReaderConfiguration->SetUINT32(MF_LOW_LATENCY, true)))
		Error::Message(hr, "Failed to set source reader configuration");

	// create XAudio2 device and mastering voice
	if (FAILED(hr = XAudio2Create(m_audioDevice.GetAddressOf())))
		Error::Message(hr, "Failed to create XAUDIO2 device");

	if (FAILED(hr = m_audioDevice->CreateMasteringVoice(&m_masteringVoice)))
		Error::Message(hr, "Failed to create XAUDIO2 mastering voice");
}

AudioEngine::~AudioEngine()
{
	MFShutdown();
	m_masteringVoice->DestroyVoice();
	m_audioDevice->StopEngine();
}
