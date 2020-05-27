#pragma once

#include "GameObject.hpp"
#include "Audio/AudioComponent.hpp"
#include "Audio/Sound.hpp"

// Game.hpp
// Handles all gameplay logic

constexpr DirectX::XMFLOAT2 PADDLE_SIZE = { 50.f, 5.f };
constexpr DirectX::XMFLOAT2 SQUARE_SIZE = { 2.5f, 2.5f };
constexpr float PADDLE_OFFSET = 10.f;

class Game
{
public:
	Game(int width, int height, AudioComponent* audio);
	void HandleInput(unsigned char keycode);
	void Update(float delta);
	void Render(Renderer& renderer);
private:
	void reset();
	void checkCollisions();

	unsigned int m_gameWidth, m_gameHeight;
	std::vector<GameObject*> m_paddles;

	GameObject m_bottomPaddle, m_topPaddle, m_square;
	AudioComponent* m_audio;
};
