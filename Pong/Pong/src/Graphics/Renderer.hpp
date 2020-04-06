#pragma once

#include "AdapterReader.hpp"

class Renderer
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void Render();
private:
	bool initD3D(HWND hwnd, int width, int height);

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};