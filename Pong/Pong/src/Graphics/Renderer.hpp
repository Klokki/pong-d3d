#pragma once

#include "Shader/Shader.hpp"
#include "Adapter/Adapter.hpp"

// Renderer.hpp
// Contains D3D objects like the D3D11Device and context, and renders to screen

class Renderer {
public:
	Renderer(HWND hwnd, int width, int height);
	void Render();
	void ToggleFillMode();
private:
	void initializeD3D(HWND hwnd, int width, int height);
	void initializeShaders();
	void initializeScene();

	std::wstring getOutputPath();

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	std::vector<AdapterData> m_adapters;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	D3D11_RASTERIZER_DESC m_rasterizerDescription;
};