#include "pch.h"
#include "D3DObject.hpp"
#include "AdapterReader.hpp"

Microsoft::WRL::ComPtr<ID3D11Device> D3DObject::m_device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3DObject::m_deviceContext;
Microsoft::WRL::ComPtr<IDXGISwapChain> D3DObject::m_swapchain;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> D3DObject::m_renderTargetView;

void D3DObject::InitializeD3D(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

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
