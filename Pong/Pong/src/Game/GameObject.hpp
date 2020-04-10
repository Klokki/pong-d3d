#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	void Update(unsigned char keycode) {
		if (keycode == 0x41 && m_position.x > 0.0f)
			m_position.x -= 5.f;
		if (keycode == 0x44 && m_position.x < 800.f)
			m_position.x += 5.f;
	}
	void Draw(Renderer& renderer) { renderer.Render(m_position, m_size); }
private:
	DirectX::XMFLOAT2 m_position;
	DirectX::XMFLOAT2 m_size;
};