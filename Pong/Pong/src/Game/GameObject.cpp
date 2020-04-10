#include "pch.h"
#include "GameObject.hpp"

GameObject::GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	:
	m_position(position),
	m_size(size)
{
}
