#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height, AudioComponent* audio)
    :
    m_gameWidth(width), m_gameHeight(height),
    m_bottomPaddle({ (float)width / 2, 10.0f }, PADDLE_SIZE),
    m_topPaddle({ (float)width / 2, (float)height - 10.0f }, PADDLE_SIZE),
    m_square({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y - SQUARE_SIZE.y / 2 }, SQUARE_SIZE)
{
    m_square.SetStuck(true);
    m_audio = audio;
}

void Game::HandleInput(unsigned char keycode, float delta)
{
    // move paddles except when touching the edges of the screen
    if (keycode == 0x41 && m_bottomPaddle.GetPosition().x > m_bottomPaddle.GetSize().x / 2)
        m_bottomPaddle.Move({ -1.f * delta, 0.f * delta });
    if (keycode == VK_LEFT && m_topPaddle.GetPosition().x > m_topPaddle.GetSize().x / 2)
        m_topPaddle.Move({ -1.f * delta, 0.f * delta });
    if (keycode == 0x44 && m_bottomPaddle.GetPosition().x < m_gameWidth - m_bottomPaddle.GetSize().x / 2)
        m_bottomPaddle.Move({ 1.f * delta, 0.f * delta });
    if (keycode == VK_RIGHT && m_topPaddle.GetPosition().x < m_gameWidth - m_topPaddle.GetSize().x / 2)
        m_topPaddle.Move({ 1.f * delta, 0.f * delta });

    // if space is pressed, unstuck the ball
    if (keycode == VK_SPACE && m_square.IsStuck())
    {
        m_square.SetStuck(false);
        m_audio->PlaySound("test");
    }
}

void Game::Update(float delta)
{
    // when "ball" is stuck, move with the paddle, when it's not, move normally
    if (m_square.IsStuck())
        m_square.SetPosition({ m_bottomPaddle.GetPosition().x, m_square.GetPosition().y });
    else
        m_square.Move({ 0.f * delta, 0.1f * delta });

    // if the ball goes outside the playfield y axis, reset
    if (m_square.GetPosition().y > m_gameHeight || m_square.GetPosition().y < 0.f)
        reset();
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
    m_bottomPaddle.SetPosition({ (float)m_gameWidth / 2, m_bottomPaddle.GetPosition().y });
    m_topPaddle.SetPosition({ (float)m_gameWidth / 2, m_topPaddle.GetPosition().y });
    m_square.SetStuck(true);
    m_square.SetPosition({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y - SQUARE_SIZE.y / 2 });

    m_audio->PlaySound("test");
}
