#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height)
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
