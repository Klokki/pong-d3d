#include "pch.h"
#include "Renderer.hpp"
#include "Utility/OutputPath.hpp"

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
	float bgcolor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::Render(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT2 size)
{
	// set input layout, depth stencil and shaders to context
	m_deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// update constant buffer
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationZ(0.0f);
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(1.0f * size.x, 1.0f * size.y, 1.0f);

	m_constantBufferData.move = scaling * rotation * translation;
	m_constantBufferData.move = DirectX::XMMatrixTranspose(m_constantBufferData.move);

	m_constantBufferData.color = color;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &m_constantBufferData, sizeof(ConstantBuffer_Vertex));
	m_deviceContext->Unmap(m_constantBuffer.Get(), 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

	// set vertex and index buffers and draw
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->DrawIndexed(6, 0, 0);
}

void Renderer::EndRender(bool vsync)
{
	m_swapchain->Present(vsync, NULL);
}

void Renderer::ToggleFillMode()
{
	HRESULT hr;

	// toggle rasterizer fill mode
	if (m_rasterizerDescription.FillMode == D3D11_FILL_MODE::D3D11_FILL_SOLID)
		m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	else
		m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	if (FAILED(hr = m_device->CreateRasterizerState(&m_rasterizerDescription, m_rasterizerState.GetAddressOf())))
		Error::Message(hr, "Could not switch rasterizer state");
}

void Renderer::initializeD3D(HWND hwnd)
{
	std::vector<AdapterData> adapters = Adapters::Get();

	if (adapters.size() < 1)
		Error::Message("No DXGI adapters found");

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

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
		Error::Message(hr, "Failed to create device and swapchain");

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	if (FAILED(hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
		Error::Message(hr, "GetBuffer failed");

	if (FAILED(hr = m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf())))
		Error::Message(hr, "Failed to create render target view");

	// create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilViewDescription = {};
	depthStencilViewDescription.Width = m_width;
	depthStencilViewDescription.Height = m_height;
	depthStencilViewDescription.MipLevels = 1;
	depthStencilViewDescription.ArraySize = 1;
	depthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescription.SampleDesc.Count = 1;
	depthStencilViewDescription.SampleDesc.Quality = 0;
	depthStencilViewDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilViewDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilViewDescription.CPUAccessFlags = 0;
	depthStencilViewDescription.MiscFlags = 0;

	if (FAILED(hr = m_device->CreateTexture2D(&depthStencilViewDescription, NULL, m_depthStencilBuffer.GetAddressOf())))
		Error::Message(hr, "Failed to create depth stencil buffer");

	if (FAILED(hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), NULL, m_depthStencilView.GetAddressOf())))
		Error::Message(hr, "Failed to create depth stencil view");

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription = {};
	depthStencilStateDescription.DepthEnable = true;
	depthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDescription.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	if (FAILED(hr = m_device->CreateDepthStencilState(&depthStencilStateDescription, m_depthStencilState.GetAddressOf())))
		Error::Message(hr, "Failed to create depth stencil state");

	// rasterizer (viewport)
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)m_width;
	viewport.Height = (FLOAT)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	// rasterizer state
	m_rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	m_rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	if (FAILED(hr = m_device->CreateRasterizerState(&m_rasterizerDescription, m_rasterizerState.GetAddressOf())))
		Error::Message(hr, "Failed to create rasterizer state");
}

void Renderer::initializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[2] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	std::wstring outputPath = OutputPath::get_path();

	// init shaders
	m_vertexShader.Initialize(m_device, outputPath + L"\\shaders\\Vertex.cso", layout, numElements);
	m_pixelShader.Initialize(m_device, outputPath + L"\\shaders\\Pixel.cso");
}

void Renderer::initializeRenderData()
{
	Vertex vertices[4] =
	{
		Vertex(-1.f, -1.f, 1.f, 1.f, 1.f, 1.f), // Bottom left
		Vertex(-1.f, 1.f, 1.f, 1.f, 1.f, 1.f), // Top left
		Vertex(1.f, 1.f, 1.f, 1.f, 1.f, 1.f), // Top right
		Vertex(1.f, -1.f, 1.f, 1.f, 1.f, 1.f), // Bottom right
	};

	DWORD indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	UINT vertexSize = sizeof(Vertex); // avoid warning C6260 (sizeof * sizeof)

	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexSize * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};

	vertexBufferData.pSysMem = vertices;

	HRESULT hr;

	if (FAILED(hr = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf())))
		Error::Message(hr, "Failed to create vertex buffer");

	// index buffer
	D3D11_BUFFER_DESC indexBufferDescription = {};

	int dwordSize = sizeof(DWORD); // avoid warning C6260 (sizeof * sizeof)

	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = dwordSize * ARRAYSIZE(indices);
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices;

	if (FAILED(hr = m_device->CreateBuffer(&indexBufferDescription, &indexBufferData, m_indexBuffer.GetAddressOf())))
		Error::Message(hr, "Failed to create index buffer");

	// constant buffer
	D3D11_BUFFER_DESC cBufferDescription = {};

	cBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDescription.MiscFlags = 0;
	cBufferDescription.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer_Vertex) + (16 - (sizeof(ConstantBuffer_Vertex) % 16)));
	cBufferDescription.StructureByteStride = 0;

	if (FAILED(hr = m_device->CreateBuffer(&cBufferDescription, 0, m_constantBuffer.GetAddressOf())))
		Error::Message(hr, "Failed to create constant buffer");

	// set orthographic projection
	DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, (float)m_width, 0.0f, (float)m_height, 0.0f, 100.0f);

	m_constantBufferData.projection = projection;
	m_constantBufferData.projection = DirectX::XMMatrixTranspose(m_constantBufferData.projection);
}
