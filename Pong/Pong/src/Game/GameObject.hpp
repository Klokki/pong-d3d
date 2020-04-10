#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject(DirectX::XMFLOAT2 position);
	void Update(unsigned char keycode) {
		if (keycode == 0x41 && m_position.x > -0.5f)
			m_position.x -= 0.05f;
		if (keycode == 0x44 && m_position.x < 0.5f)
			m_position.x += 0.05f;
	}
	void Draw(Renderer& renderer) { renderer.Render(m_position); }
private:
	DirectX::XMFLOAT2 m_position;
};