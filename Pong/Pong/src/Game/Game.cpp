#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height),
	m_testObject({(float)width / 2, (float)height / 2},
		{100.0f, 50.0f})
{
}

void Game::Update(unsigned char keycode)
{
	m_testObject.Update(keycode);
}

void Game::Render(Renderer& renderer)
{
	m_testObject.Draw(renderer);
}
