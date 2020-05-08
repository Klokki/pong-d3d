#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size)
	:
	m_position(position),
	m_size(size),
	m_boundingBox({ m_position.x, m_position.y, m_position.z }, { m_size.x, m_size.y, 1.f })
{
}


GameObject::GameObject(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT2 size)
	:
	m_color(color),
	m_position(position),
	m_size(size),
	m_boundingBox({ m_position.x, m_position.y, m_position.z }, { m_size.x / 2, m_size.y / 2, 1.f })
{
}

void GameObject::Update(float delta)
{
	m_position.x += m_velocity.x * delta;
	m_position.y += m_velocity.y * delta;

	// also move bounding box
	m_boundingBox.Center = { m_position.x, m_position.y, m_position.z };
}

void GameObject::Render(Renderer& renderer)
{
	renderer.Render(m_position, m_color, m_size);
}
