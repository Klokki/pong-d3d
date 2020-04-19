#pragma once

#include "GameObject.hpp"

// Game.hpp
// Handles all gameplay logic

constexpr DirectX::XMFLOAT2 PADDLE_SIZE = { 100.0f, 10.0f };

class Game
{
public:
	Game(int width, int height);
	void Update(unsigned char keycode, float delta);
	void Render(Renderer& renderer);
private:
	int m_gameWidth, m_gameHeight;
	GameObject m_testObject, m_testObject2, m_testObject3;
};