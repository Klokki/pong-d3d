#pragma once

#include "Shader/Shader.hpp"

// D3DObject.hpp
// Contains D3D objects like the D3D11Device and context
// Also has information about the adapters in the system

class AdapterData
{
	friend class D3DObject;
public:
	IDXGIAdapter* GetAdapter() const { return p_adapter; }
protected:
	AdapterData(IDXGIAdapter* adapter);
	IDXGIAdapter* p_adapter = nullptr;
	DXGI_ADAPTER_DESC m_description;
};

class D3DObject {
public:
	static void InitializeD3D(HWND hwnd, int width, int height);
	static void InitializeShaders();
	static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_device; }
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_deviceContext; }
	static Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() { return m_swapchain; }
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return m_renderTargetView; }

private:
	static std::vector<AdapterData> getAdapters();

	static Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	static VertexShader m_vertexShader;

	static std::vector<AdapterData> m_adapters;
};