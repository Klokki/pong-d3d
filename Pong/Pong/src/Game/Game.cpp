#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height, AudioComponent* audio)
	:
	m_gameWidth(width), m_gameHeight(height),
	m_bottomPaddle({ (float)width / 2, PADDLE_OFFSET, 0.f }, PADDLE_SIZE),
	m_topPaddle({ (float)width / 2, (float)height - PADDLE_OFFSET, 0.f }, PADDLE_SIZE),
	m_square({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y + SQUARE_SIZE.y / 2, 0.f }, { 0.5f, 1.f, 0.f }, SQUARE_SIZE)
{
	m_square.SetStuck(true);
	m_audio = audio;

	m_paddles.push_back(&m_bottomPaddle);
	m_paddles.push_back(&m_topPaddle);
}

void Game::HandleInput(unsigned char keycode)
{
	// move paddles except when touching the edges of the screen
	if (keycode == 0x41 && m_bottomPaddle.GetPosition().x > m_bottomPaddle.GetSize().x)
		m_bottomPaddle.SetVelocity({ -0.5f, 0.f });
	if (keycode == VK_LEFT && m_topPaddle.GetPosition().x > m_topPaddle.GetSize().x)
		m_topPaddle.SetVelocity({ -0.5f, 0.f });
	if (keycode == 0x44 && m_bottomPaddle.GetPosition().x < m_gameWidth - m_bottomPaddle.GetSize().x)
		m_bottomPaddle.SetVelocity({ 0.5f, 0.f });
	if (keycode == VK_RIGHT && m_topPaddle.GetPosition().x < m_gameWidth - m_topPaddle.GetSize().x)
		m_topPaddle.SetVelocity({ 0.5f, 0.f });

	// if space is pressed, unstuck the ball
	if (keycode == VK_SPACE && m_square.IsStuck())
	{
		m_square.SetStuck(false);
		m_square.SetVelocity({ m_square.GetVelocity().x, 0.5f });

		m_audio->PlaySound("bleep1");
	}
}

void Game::Update(float delta)
{
	// if the ball goes outside the playfield y axis, reset
	if (m_square.GetPosition().y > m_gameHeight || m_square.GetPosition().y < 0.f)
		reset();

	if (m_square.IsStuck())
		m_square.SetVelocity(m_bottomPaddle.GetVelocity());

	m_bottomPaddle.Update(delta);
	m_topPaddle.Update(delta);
	m_square.Update(delta);

	checkCollisions();

	// set velocity to zero every frame to prevent drifting
	m_bottomPaddle.SetVelocity({ 0.f, 0.f });
	m_topPaddle.SetVelocity({ 0.f, 0.f });
}

void Game::Render(Renderer& renderer)
{
	m_bottomPaddle.Render(renderer);
	m_topPaddle.Render(renderer);
	m_square.Render(renderer);
}

void Game::reset()
{
	// reset playfield to original positions
	m_bottomPaddle.SetPosition({ (float)m_gameWidth / 2, m_bottomPaddle.GetPosition().y, m_bottomPaddle.GetPosition().z });
	m_topPaddle.SetPosition({ (float)m_gameWidth / 2, m_topPaddle.GetPosition().y, m_topPaddle.GetPosition().z });

	m_square.SetStuck(true);
	m_square.SetPosition({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y + SQUARE_SIZE.y / 2, m_square.GetPosition().z });
	m_square.SetVelocity({ 0.f, 0.f });

	m_audio->StopSounds(); // stop sounds to alleviate some problems with buggy audio
	m_audio->PlaySound("bleep1");
}

void Game::checkCollisions()
{
	bool localCollisionFlag = false;

	// iterate through both paddles for collision detection
	for (GameObject* paddle : m_paddles)
	{
		// if the ball is colliding with a paddle, change the velocity
		if (m_square.IsColliding(*paddle))
		{
			// hitting the left side of the paddle moves the ball towards negative x coordinates and vice versa
			if (m_square.GetPosition().x < paddle->GetPosition().x)
				m_square.SetVelocity({ -0.25f, -m_square.GetVelocity().y });
			else if (m_square.GetPosition().x > paddle->GetPosition().x)
				m_square.SetVelocity({ 0.25f, -m_square.GetVelocity().y });
			else if (m_square.GetPosition().x == paddle->GetPosition().x)
				m_square.SetVelocity({ 0, -m_square.GetVelocity().y });

			if (!m_square.WasColliding())
				m_audio->PlaySound("bleep2");

			m_square.SetColliding(true);
				localCollisionFlag = true;
		}
	}

	// window edge collision
	if (m_square.GetPosition().x <= 0.f || m_square.GetPosition().x >= m_gameWidth)
	{
		m_square.SetVelocity({ -m_square.GetVelocity().x, m_square.GetVelocity().y });

		if (!m_square.WasColliding())
			m_audio->PlaySound("bleep3");

		m_square.SetColliding(true);
		localCollisionFlag = true;
	}

	if (!localCollisionFlag)
		m_square.SetColliding(false);
}
