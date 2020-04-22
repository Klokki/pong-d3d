#pragma once

#include "GameObject.hpp"
#include "Audio/AudioComponent.hpp"
#include "Audio/Sound.hpp"

// Game.hpp
// Handles all gameplay logic

constexpr DirectX::XMFLOAT2 PADDLE_SIZE = { 100.f, 10.f };
constexpr DirectX::XMFLOAT2 SQUARE_SIZE = { 5.f, 5.f };

class Game
{
public:
    Game(int width, int height, AudioComponent* audio);
    void HandleInput(unsigned char keycode, float delta);
    void Update(float delta);
    void Render(Renderer& renderer);
private:
    void reset();
    bool checkCollision(GameObject& obj1, GameObject& obj2);

    int m_gameWidth, m_gameHeight;
    GameObject m_bottomPaddle, m_topPaddle, m_square;
    AudioComponent* m_audio;
};