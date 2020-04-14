#pragma once

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();
private:
	Microsoft::WRL::ComPtr<IXAudio2> m_audioDevice;
	IXAudio2MasteringVoice* m_masteringVoice;
};