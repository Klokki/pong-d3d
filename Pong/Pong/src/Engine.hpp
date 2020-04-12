#pragma once

#include "Window/Window.hpp"
#include "Input/Keyboard.hpp"
#include "Graphics/Renderer.hpp"
#include "Game/Game.hpp"

class Engine
{
public:
	Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);

	bool ProcessMessages()
	{
		return m_window.ProcessMessages();
	}

	void Update(float delta);
	void Render();
private:
	bool m_vsync = true;
	Window m_window;
	Keyboard m_keyboard;
	Renderer m_renderer;
	Game m_game;
};