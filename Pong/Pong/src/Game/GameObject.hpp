#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	void Move(DirectX::XMFLOAT2 value);
	void Draw(Renderer& renderer) { renderer.Render(m_position, m_size); }

	void SetStuck(bool value) { m_stuck = value; }
	bool IsStuck() { return m_stuck; }
	DirectX::XMFLOAT2 GetPosition() { return m_position; }
	DirectX::XMFLOAT2 GetSize() { return m_size; }
	void SetPosition(float position) { m_position.x = position; }
private:
	DirectX::XMFLOAT2 m_position;
	DirectX::XMFLOAT2 m_size;
	bool m_stuck = false;
};