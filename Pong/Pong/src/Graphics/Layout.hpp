#pragma once

#include "pch.h"

// vertex shader constant buffer
struct CB_VS
{
	DirectX::XMMATRIX move;
	DirectX::XMMATRIX projection;
};

// vertex buffer layout
struct Vertex
{
	Vertex(float x, float y, float z, float r, float g, float b)
		: position(x, y, z), color(r, g, b) {}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};
