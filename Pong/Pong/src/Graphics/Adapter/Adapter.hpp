#pragma once

class AdapterData
{
	friend class Adapters;
public:
	IDXGIAdapter* GetAdapter() const { return p_adapter; }
protected:
	AdapterData(IDXGIAdapter* adapter);
	IDXGIAdapter* p_adapter = nullptr;
	DXGI_ADAPTER_DESC m_description;
};

class Adapters
{
public:
	static std::vector<AdapterData> Get();
private:
	static std::vector<AdapterData> m_adapters;
};