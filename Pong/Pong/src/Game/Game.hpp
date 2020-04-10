#pragma once

#include "GameObject.hpp"

// Game.hpp
// Handles all gameplay logic

class Game
{
public:
	Game(int width, int height);
	void Update(unsigned char keycode);
	void Render(Renderer& renderer);
private:
	int m_gameWidth, m_gameHeight;
	GameObject m_testObject;
};