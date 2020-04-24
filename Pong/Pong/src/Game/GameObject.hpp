#pragma once

#include "../Graphics/Renderer.hpp"

class GameObject
{
public:
    GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
    void Update(float delta);
    void Render(Renderer& renderer);

    bool IsStuck() { return m_stuck; }
    DirectX::XMFLOAT2 GetPosition() { return m_position; }
    DirectX::XMFLOAT2 GetSize() { return m_size; }
    DirectX::XMFLOAT2 GetVelocity() { return m_velocity; }

    void SetPosition(DirectX::XMFLOAT2 position) { m_position = position; }
    void SetStuck(bool value) { m_stuck = value; }
    void SetVelocity(DirectX::XMFLOAT2 value) { m_velocity = value; }
private:
    DirectX::XMFLOAT2 m_position;
    DirectX::XMFLOAT2 m_size;
    DirectX::XMFLOAT2 m_velocity = { 0.f, 0.f };
    bool m_stuck = false;
};
