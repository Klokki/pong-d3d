#include "pch.h"
#include "Engine.hpp"

Engine::Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	:
	m_window(this, hInstance, window_title, window_class, width, height),
	m_renderer(m_window.GetHWND(), width, height),
	m_game(width, height)
{
}

void Engine::Update()
{
	m_keyboard.Update();
	unsigned char keycode = NULL;

	while (!m_keyboard.KeyBufferEmpty())
	{
		KeyboardEvent e = m_keyboard.ReadBuffer();
		keycode = e.GetKeyCode();

		// close window on ESC
		if (keycode == VK_ESCAPE)
			PostMessage(m_window.GetHWND(), WM_CLOSE, NULL, NULL);

		// switch fill mode (solid/wireframe) on keyup
		if (keycode == VK_F1 && e.KeyUp() == true)
			m_renderer.ToggleFillMode();
	}

	m_game.Update(keycode);
}

void Engine::Render()
{
	m_renderer.BeginRender();
	m_game.Render(m_renderer);
	m_renderer.EndRender();
}