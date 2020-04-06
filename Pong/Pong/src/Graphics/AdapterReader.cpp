#include "pch.h"
#include "AdapterReader.hpp"

std::vector<AdapterData> AdapterReader::m_adapters;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	// if adapters have already been retrieved, return m_adapters
	if (m_adapters.size() > 0)
		return m_adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> p_factory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(p_factory.GetAddressOf()));
	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create DXGIFactory for enumerating adapters");
		exit(EXIT_FAILURE);
	}

	IDXGIAdapter* p_adapter;
	UINT i = 0;

	// add the data of the adapter to m_adapters vector
	while (SUCCEEDED(p_factory->EnumAdapters(i, &p_adapter)))
	{
		m_adapters.push_back(AdapterData(p_adapter));
		++i;
	}

	return m_adapters;
}

AdapterData::AdapterData(IDXGIAdapter* adapter)
{
	p_adapter = adapter;
	HRESULT hr = p_adapter->GetDesc(&m_description);
	if (FAILED(hr))
		Error::Message(hr, "Failed to Get Description for IDXGIAdapter.");
}