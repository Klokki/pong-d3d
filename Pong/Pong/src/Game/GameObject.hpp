#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
	GameObject(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size);
	GameObject(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT2 size);

	void Update(float delta);
	void Render(Renderer& renderer);

	bool IsStuck() const { return m_stuck; }
	DirectX::XMFLOAT3 GetPosition() const { return m_position; }
	DirectX::XMFLOAT2 GetSize() const { return m_size; }
	DirectX::XMFLOAT2 GetVelocity() const { return m_velocity; }
	bool IsColliding(GameObject& object) const { return m_boundingBox.Contains(object.m_boundingBox); }
	bool WasColliding() const { return m_collided; }

	void SetPosition(DirectX::XMFLOAT3 position) { m_position = position; }
	void SetStuck(bool value) { m_stuck = value; }
	void SetVelocity(DirectX::XMFLOAT2 value) { m_velocity = value; }
	void SetColliding(bool colliding) { m_collided = colliding; }
private:
	DirectX::XMFLOAT3 m_color = { 1.f, 1.f, 1.f };
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_size;
	DirectX::XMFLOAT2 m_velocity = { 0.f, 0.f };
	DirectX::BoundingBox m_boundingBox;

	bool m_stuck = false;
	bool m_collided = false;
};
