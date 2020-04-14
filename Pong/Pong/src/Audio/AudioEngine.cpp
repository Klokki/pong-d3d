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

void AudioEngine::loadFile(const std::wstring& filename, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormatEx, unsigned int& waveFormatLength)
{
	HRESULT hr;

	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	// create source reader
	Microsoft::WRL::ComPtr<IMFSourceReader> sourceReader;

	if(FAILED(hr = MFCreateSourceReaderFromURL(filename.c_str(), m_sourceReaderConfiguration.Get(), sourceReader.GetAddressOf())))
		Error::Message(hr, "Failed to create source reader from URL");

	// deselect all audio streams and select the first one
	if (FAILED(hr = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false)))
		Error::Message(hr, "Failed to deselect all streams");

	if (FAILED(hr = sourceReader->SetStreamSelection(streamIndex, true)))
		Error::Message(hr, "Failed to set stream to index");

	// get information about media file
	Microsoft::WRL::ComPtr<IMFMediaType> nativeMediaType;

	if (FAILED(hr = sourceReader->GetNativeMediaType(streamIndex, 0, nativeMediaType.GetAddressOf())))
		Error::Message(hr, "Failed to get native media type");

	// check that the file is an audio file
	GUID majorType{};
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);

	if (majorType != MFMediaType_Audio)
		Error::Message(hr, "File was not an audio file");

	// check if the file is uncompressed or not
	GUID subType{};
	hr = nativeMediaType->GetGUID(MF_MT_SUBTYPE, &subType);

	if (subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM)
	{
		// File is uncompressed
	}
	else
	{
		// decompress audio file
		Microsoft::WRL::ComPtr<IMFMediaType> partialType = nullptr;

		if (FAILED(hr = MFCreateMediaType(partialType.GetAddressOf())))
			Error::Message(hr, "Failed to create media type");

		// set media type (audio)
		if (FAILED(hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)))
			Error::Message(hr, "Failed to set major media type");

		if (FAILED(hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)))
			Error::Message(hr, "Failed to set media subtype to uncompressed");

		if (FAILED(hr = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType.Get())))
			Error::Message(hr, "Failed to set current media type");
	}

	// uncompress data and load into XAudio buffer
	Microsoft::WRL::ComPtr<IMFMediaType> uncompressedType = nullptr;

	if (FAILED(hr = MFCreateWaveFormatExFromMFMediaType(uncompressedType.Get(), waveFormatEx, &waveFormatLength)))
		Error::Message(hr, "Failed to create wave format");

	if (FAILED(hr = sourceReader->SetStreamSelection(streamIndex, true)))
		Error::Message(hr, "Failed to ensure stream selection");

	Microsoft::WRL::ComPtr<IMFSample> sample = nullptr;
	Microsoft::WRL::ComPtr<IMFMediaBuffer> buffer = nullptr;
	BYTE* localAudioData = NULL;
	DWORD localAudioDataLength = 0;

	while (true)
	{
		DWORD flags = 0;

		if (FAILED(hr = sourceReader->ReadSample(streamIndex, 0, nullptr, &flags, nullptr, sample.GetAddressOf())))
			Error::Message(hr, "Failed to read sample");

		// check data validity
		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
			break;

		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
			break;

		if (sample == nullptr)
			continue;

		if (FAILED(hr = sample->ConvertToContiguousBuffer(buffer.GetAddressOf())))
			Error::Message(hr, "Failed to convert audio to contiguous buffer");

		if (FAILED(hr = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength)))
			Error::Message(hr, "Failed to lock audio buffer");

		for (size_t i = 0; i < localAudioDataLength; i++)
			audioData.push_back(localAudioData[i]);

		hr = buffer->Unlock();
		localAudioData = nullptr;

		if (FAILED(hr))
			Error::Message(hr, "Failed to unlock audio buffer");
	}
}
