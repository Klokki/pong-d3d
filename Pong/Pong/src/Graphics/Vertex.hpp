#pragma once

#include "pch.h"

struct Vertex
{
	Vertex(float x, float y)
		: position(x, y) {}

	DirectX::XMFLOAT2 position;
};