#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height)
	:
	m_gameWidth(width), m_gameHeight(height),
	m_testObject({ (float)width / 2, 10.0f }, PADDLE_SIZE),
	m_testObject2({ (float)width / 2, (float)height - 10.0f }, PADDLE_SIZE),
	m_testObject3({ (float)width / 2, (float)height / 2 }, { 5.f, 5.f })
{
}

void Game::Update(unsigned char keycode, float delta)
{
	if (keycode == 0x41 || keycode == 0x44)
		m_testObject.Update(keycode, delta);
	else if (keycode == VK_LEFT || keycode == VK_RIGHT)
		m_testObject2.Update(keycode, delta);
}

void Game::Render(Renderer& renderer)
{
	m_testObject.Draw(renderer);
	m_testObject2.Draw(renderer);
	m_testObject3.Draw(renderer);
}
