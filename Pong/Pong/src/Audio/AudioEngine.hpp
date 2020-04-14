#pragma once

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();
private:
	Microsoft::WRL::ComPtr<IXAudio2> m_audioDevice;
	Microsoft::WRL::ComPtr<IMFAttributes> m_sourceReaderConfiguration;
	IXAudio2MasteringVoice* m_masteringVoice;
};