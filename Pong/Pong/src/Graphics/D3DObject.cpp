#include "pch.h"
#include "D3DObject.hpp"

Microsoft::WRL::ComPtr<ID3D11Device> D3DObject::m_device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3DObject::m_deviceContext;
Microsoft::WRL::ComPtr<IDXGISwapChain> D3DObject::m_swapchain;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> D3DObject::m_renderTargetView;

std::vector<AdapterData> D3DObject::m_adapters;

void D3DObject::InitializeD3D(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = getAdapters();

	if (adapters.size() < 1)
	{
		Error::Message("No DXGI adapters found");
		exit(EXIT_FAILURE);
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].GetAdapter(), // DXGI adapter
		D3D_DRIVER_TYPE_UNKNOWN, // Driver type
		NULL, // For D3D_DRIVER_TYPE_SOFTWARE
		NULL, // Layer flags
		NULL, // D3D_FEATURE_LEVELS (use default)
		0, // Number of feature levels in previous parameter
		D3D11_SDK_VERSION,
		&swapChainDesc,
		m_swapchain.GetAddressOf(),
		m_device.GetAddressOf(),
		NULL, // Supported feature level
		m_deviceContext.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create device and swapchain");
		exit(EXIT_FAILURE);
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		Error::Message(hr, "GetBuffer failed");
		exit(EXIT_FAILURE);
	}

	hr = m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create render target view");
		exit(EXIT_FAILURE);
	}

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);
}

std::vector<AdapterData> D3DObject::getAdapters()
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