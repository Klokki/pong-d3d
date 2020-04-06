#pragma once

// AdapterReader.hpp
// AdapterData stores information about a single adapter (video card)
// AdapterReader contains a vector of all adapters (video cards)
class AdapterData
{
	friend class AdapterReader;
public:
	IDXGIAdapter* GetAdapter() const { return p_adapter; }
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