#pragma once

class AdapterData
{
	friend class AdapterReader;
protected:
	AdapterData(IDXGIAdapter* adapter);
	IDXGIAdapter* p_adapter = nullptr;
	DXGI_ADAPTER_DESC m_description;
};

class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();
private:
	static std::vector<AdapterData> m_adapters;
};