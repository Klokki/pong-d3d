#pragma once

#include "pch.h"

// vertex shader constant buffer
struct ConstantBuffer_Vertex
{
	DirectX::XMMATRIX move;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT3 color;
};

// vertex buffer layout
struct Vertex
{
	Vertex(float x, float y, float z, float r, float g, float b)
		: position(x, y, z), color(r, g, b) {}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};
