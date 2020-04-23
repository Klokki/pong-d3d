#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
    :
    m_position(position),
    m_size(size)
{
}

void GameObject::Update(float delta)
{
    m_position.x += m_velocity.x * delta;
    m_position.y += m_velocity.y * delta;
}

void GameObject::Render(Renderer& renderer)
{
    renderer.Render(m_position, m_size);
}
