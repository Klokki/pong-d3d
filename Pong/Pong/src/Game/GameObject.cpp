#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
    :
    m_position(position),
    m_size(size),
    m_boundingBox({ m_position.x, m_position.y, 0.f }, { m_size.x / 2, m_size.y / 2, 0.f })
{
}

void GameObject::Update(float delta)
{
    m_position.x += m_velocity.x * delta;
    m_position.y += m_velocity.y * delta;

    // also move bounding box
    m_boundingBox.Center = { m_position.x, m_position.y, 0.f };
}

void GameObject::Render(Renderer& renderer)
{
    renderer.Render(m_position, m_size);
}
