#pragma once

#include "Shader/Shader.hpp"

// D3DObject.hpp
// Contains D3D objects like the D3D11Device and context
// Also has information about the adapters in the system

class AdapterData
{
	friend class Renderer;
public:
	IDXGIAdapter* GetAdapter() const { return p_adapter; }
protected:
	AdapterData(IDXGIAdapter* adapter);
	IDXGIAdapter* p_adapter = nullptr;
	DXGI_ADAPTER_DESC m_description;
};

class Renderer {
public:
	Renderer(HWND hwnd, int width, int height);
	void Render();
	void ToggleFillMode();
private:
	void initializeD3D(HWND hwnd, int width, int height);
	void initializeShaders();
	void initializeScene();

	std::vector<AdapterData> getAdapters();
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