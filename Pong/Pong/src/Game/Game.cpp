#include "pch.h"
#include "Game.hpp"

Game::Game(int width, int height, AudioComponent* audio)
    :
    m_gameWidth(width), m_gameHeight(height),
    m_bottomPaddle({ (float)width / 2, PADDLE_OFFSET }, PADDLE_SIZE),
    m_topPaddle({ (float)width / 2, (float)height - PADDLE_OFFSET }, PADDLE_SIZE),
    m_square({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y - SQUARE_SIZE.y / 2 }, SQUARE_SIZE)
{
    m_square.SetStuck(true);
    m_audio = audio;
}

void Game::HandleInput(unsigned char keycode)
{
    // move paddles except when touching the edges of the screen
    if (keycode == 0x41 && m_bottomPaddle.GetPosition().x > m_bottomPaddle.GetSize().x / 2)
        m_bottomPaddle.SetVelocity({ -0.5f, 0.f });
    if (keycode == VK_LEFT && m_topPaddle.GetPosition().x > m_topPaddle.GetSize().x / 2)
        m_topPaddle.SetVelocity({ -0.5f, 0.f });
    if (keycode == 0x44 && m_bottomPaddle.GetPosition().x < m_gameWidth - m_bottomPaddle.GetSize().x / 2)
        m_bottomPaddle.SetVelocity({ 0.5f, 0.f });
    if (keycode == VK_RIGHT && m_topPaddle.GetPosition().x < m_gameWidth - m_topPaddle.GetSize().x / 2)
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
    m_bottomPaddle.SetPosition({ (float)m_gameWidth / 2, m_bottomPaddle.GetPosition().y });
    m_topPaddle.SetPosition({ (float)m_gameWidth / 2, m_topPaddle.GetPosition().y });

    m_square.SetStuck(true);
    m_square.SetPosition({ m_bottomPaddle.GetPosition().x, m_bottomPaddle.GetPosition().y + m_bottomPaddle.GetSize().y - SQUARE_SIZE.y / 2 });
    m_square.SetVelocity({ 0.f, 0.f });

    m_audio->StopSounds(); // stop sounds to alleviate some problems with buggy audio
    m_audio->PlaySound("bleep1");
}

void Game::checkCollisions()
{
    // check collisions and on collision reverse ball direction maybe add the paddles to a vector and iterate through them but idgaf
    if (m_square.IsColliding(m_topPaddle))
    {
        m_square.SetVelocity({ m_topPaddle.GetVelocity().x, -m_square.GetVelocity().y });
        m_audio->PlaySound("bleep2");
    }
    else if (m_square.IsColliding(m_bottomPaddle) && !m_square.IsStuck())
    {
        m_square.SetVelocity({ m_bottomPaddle.GetVelocity().x, -m_square.GetVelocity().y });
        m_audio->PlaySound("bleep2");
    }

    if (m_square.GetPosition().x <= 0.f || m_square.GetPosition().x >= m_gameWidth)
    {
        m_square.SetVelocity({ -m_square.GetVelocity().x, m_square.GetVelocity().y });
        m_audio->PlaySound("bleep3");
    }
}
