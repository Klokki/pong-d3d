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
    Vertex(float x, float y, float r, float g, float b)
        : position(x, y), color(r, g, b) {}

    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT3 color;
};
