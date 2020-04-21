#pragma once

#include "GameObject.hpp"

// Game.hpp
// Handles all gameplay logic

constexpr DirectX::XMFLOAT2 PADDLE_SIZE = { 100.f, 10.f };
constexpr DirectX::XMFLOAT2 SQUARE_SIZE = { 5.f, 5.f };

class Game
{
public:
	Game(int width, int height);
	void HandleInput(unsigned char keycode, float delta);
	void Update(float delta);
	void Render(Renderer& renderer);
private:
	int m_gameWidth, m_gameHeight;
	GameObject m_bottomPaddle, m_topPaddle, m_square;
};