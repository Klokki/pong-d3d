#pragma once

class AudioEngine
{
    friend class AudioComponent;
public:
    AudioEngine();
    ~AudioEngine();
private:
    void loadFile(const std::wstring& filename, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormatEx, unsigned int& waveFormatLength);

    Microsoft::WRL::ComPtr<IXAudio2> m_audioDevice;
    Microsoft::WRL::ComPtr<IMFAttributes> m_sourceReaderConfiguration;
    IXAudio2MasteringVoice* m_masteringVoice;
};