#pragma once

class D3DObject {
public:
	static void InitializeD3D(HWND hwnd, int width, int height);
	static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_device; }
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_deviceContext; }
	static Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() { return m_swapchain; }
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return m_renderTargetView; }

private:
	static Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};