#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	:
	m_position(position),
	m_size(size)
{
}

void GameObject::Move(DirectX::XMFLOAT2 value)
{
	m_position.x += value.x;
	m_position.y += value.y;
}

void GameObject::Draw(Renderer& renderer)
{
	renderer.Render(m_position, m_size);
}
