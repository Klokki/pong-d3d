#include "pch.h"
#include "Renderer.hpp"

Renderer::Renderer(HWND hwnd, int width, int height)
	:
	m_width(width), m_height(height)
{
	initializeD3D(hwnd);
	initializeShaders();
	initializeRenderData();
}

void Renderer::BeginRender()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::Render(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
{
	// update constant buffer
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationZ(0.0f);
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(1.0f * size.x, 1.0f * size.y, 1.0f);

	m_constantBufferData.move = scaling * rotation * translation;
	m_constantBufferData.move = DirectX::XMMatrixTranspose(m_constantBufferData.move);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &m_constantBufferData, sizeof(CB_VS));
	m_deviceContext->Unmap(m_constantBuffer.Get(), 0);

	m_deviceContext->DrawIndexed(6, 0, 0);
}

void Renderer::EndRender()
{
	m_swapchain->Present(0, NULL);
}

void Renderer::ToggleFillMode()
{
	// toggle rasterizer fill mode
	if (m_rasterizerDescription.FillMode == D3D11_FILL_MODE::D3D11_FILL_SOLID)
	{
		ZeroMemory(&m_rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
		m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	}
	else
	{
		ZeroMemory(&m_rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
		m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	}

	m_rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	HRESULT hr = m_device->CreateRasterizerState(&m_rasterizerDescription, m_rasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Could not switch rasterizer state");
		exit(EXIT_FAILURE);
	}

	m_deviceContext->RSSetState(m_rasterizerState.Get());
}

void Renderer::initializeD3D(HWND hwnd)
{
	std::vector<AdapterData> adapters = Adapters::Get();

	if (adapters.size() < 1)
	{
		Error::Message("No DXGI adapters found");
		exit(EXIT_FAILURE);
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
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

	D3D11_TEXTURE2D_DESC depthStencilDescription;
	depthStencilDescription.Width = m_width;
	depthStencilDescription.Height = m_height;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	hr = m_device->CreateTexture2D(&depthStencilDescription, NULL, m_depthStencilBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create depth stencil buffer");
		exit(EXIT_FAILURE);
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), NULL, m_depthStencilView.GetAddressOf());

	// Output merger
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// rasterizer (viewport)
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)m_width;
	viewport.Height = (FLOAT)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	// rasterizer state
	ZeroMemory(&m_rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	m_rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&m_rasterizerDescription, m_rasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create rasterizer state");
		exit(EXIT_FAILURE);
	}

	m_deviceContext->RSSetState(m_rasterizerState.Get());
}

void Renderer::initializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0,
		D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};

	UINT numElements = ARRAYSIZE(layout);

	std::wstring outputPath = getOutputPath();

	// init shaders
	m_vertexShader.Initialize(m_device, outputPath + L"\\Vertex.cso", layout, numElements);
	m_pixelShader.Initialize(m_device, outputPath + L"\\Pixel.cso");
}

void Renderer::initializeRenderData()
{
	Vertex v[] =
	{
		Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 1.0f), // Bottom left
		Vertex(-0.5f, 0.5f, 1.0f, 0.0f, 0.0f), // Top left
		Vertex(0.5f, 0.5f, 1.0f, 1.0f, 0.0f), // Top right
		Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f), // Bottom right
	};

	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	UINT vertexSize = sizeof(Vertex); // avoid warning C6260 (sizeof * sizeof)

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexSize * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));

	vertexBufferData.pSysMem = v;

	HRESULT hr = m_device->CreateBuffer(&vertexBufferDesc,
		&vertexBufferData,
		m_vertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create vertex buffer");
		exit(EXIT_FAILURE);
	}

	// index buffer
	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory(&indexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	int dwordSize = sizeof(DWORD); // avoid warning C6260 (sizeof * sizeof)

	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = dwordSize * ARRAYSIZE(indices);
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices;

	hr = m_device->CreateBuffer(&indexBufferDescription, &indexBufferData,
		m_indexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create index buffer");
		exit(EXIT_FAILURE);
	}

	// constant buffer
	D3D11_BUFFER_DESC cBufferDescription;
	cBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDescription.MiscFlags = 0;
	cBufferDescription.ByteWidth = static_cast<UINT>(sizeof(CB_VS) + (16 - (sizeof(CB_VS) % 16)));
	cBufferDescription.StructureByteStride = 0;

	hr = m_device->CreateBuffer(&cBufferDescription, 0, m_constantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Error::Message(hr, "Failed to create constant buffer");
		exit(EXIT_FAILURE);
	}

	// set orthographic projection
	DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f,
		(float)m_width, 0.0f,
		(float)m_height, 0.0f, 100.0f);

	m_constantBufferData.projection = projection;
	m_constantBufferData.projection = DirectX::XMMatrixTranspose(m_constantBufferData.projection);

	// set input layout and shaders to context
	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// set vertex buffer to context
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1,
		m_vertexBuffer.GetAddressOf(),
		&stride, &offset);

	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);
	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

std::wstring Renderer::getOutputPath()
{
	// returns output directory
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);
	PathCchRemoveFileSpec(path, sizeof(path));
	return (std::wstring)path;
}