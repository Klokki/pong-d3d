#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height),
	m_testObject({(float)width / 2, 50.0f},
		{100.0f, 50.0f}),
	m_testObject2({ (float)width / 2, (float)height - 50.0f },
		{ 100.0f, 50.0f })
{
}

void Game::Update(unsigned char keycode, float delta)
{
	m_testObject.Update(keycode, delta);
}

void Game::Render(Renderer& renderer)
{
	m_testObject.Draw(renderer);
	m_testObject2.Draw(renderer);
}
