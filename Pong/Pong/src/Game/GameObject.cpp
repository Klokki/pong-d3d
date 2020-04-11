#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	:
	m_position(position),
	m_size(size)
{
}

void GameObject::Update(unsigned char keycode)
{
	if (keycode == 0x41 && m_position.x > m_size.x / 2)
		m_position.x -= 5.f;
	if (keycode == 0x44 && m_position.x < 800.f - m_size.x / 2)
		m_position.x += 5.f;
}
