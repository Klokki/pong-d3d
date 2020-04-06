#include "pch.h"
#include "Renderer.hpp"
#include "D3DObject.hpp"

void Renderer::Initialize(HWND hwnd, int width, int height)
{
	D3DObject::InitializeD3D(hwnd, width, height);
}

void Renderer::Render()
{
	float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	D3DObject::GetContext()->ClearRenderTargetView(D3DObject::GetRenderTargetView().Get(), bgcolor);
	D3DObject::GetSwapChain()->Present(1, NULL);
}
