#pragma once

#include "Layout.hpp"
#include "Shader/Shader.hpp"
#include "Adapter/Adapter.hpp"

// Renderer.hpp
// Contains D3D objects like the D3D11Device and context, and renders to screen

class Renderer {
public:
	Renderer(HWND hwnd, int width, int height);
	void BeginRender();
	void Render(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT2 size);
	void EndRender(bool vsync);
	void ToggleFillMode();
private:
	void initializeD3D(HWND hwnd);
	void initializeShaders();
	void initializeRenderData();

	std::wstring getOutputPath();

	int m_width, m_height;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	std::vector<AdapterData> m_adapters;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;
	ConstantBuffer_Vertex m_constantBufferData;

	D3D11_RASTERIZER_DESC m_rasterizerDescription = {};
};
