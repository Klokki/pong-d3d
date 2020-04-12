#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	void Update(unsigned char keycode, float delta);
	void Draw(Renderer& renderer) { renderer.Render(m_position, m_size); }
private:
	DirectX::XMFLOAT2 m_position;
	DirectX::XMFLOAT2 m_size;
};