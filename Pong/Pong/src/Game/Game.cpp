#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height), m_testObject({0.0f, -0.5f})
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
