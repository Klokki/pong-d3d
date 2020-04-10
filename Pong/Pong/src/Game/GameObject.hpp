#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject();
	void Update() {
		if (m_position.y < 1.0f)
			m_position.y += 0.01f;
		else if (m_position.y > -1.0f)
			m_position.y = -1.0f;
	}
	void Draw(Renderer& renderer) { renderer.Render(m_position); }
private:
	DirectX::XMFLOAT2 m_position;
};