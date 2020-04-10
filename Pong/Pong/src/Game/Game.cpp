#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height)
{
}

void Game::Update()
{
	m_testObject.Update();
}

void Game::Render(Renderer& renderer)
{
	m_testObject.Draw(renderer);
}
