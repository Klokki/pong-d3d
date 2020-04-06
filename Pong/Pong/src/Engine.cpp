#include "pch.h"
#include "Engine.hpp"

Engine::Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	m_window.Initialize(this, hInstance, window_title, window_class, width, height);
	m_renderer.Initialize(m_window.GetHWND(), width, height);
}

void Engine::Update()
{
	while (!m_keyboard.KeyBufferEmpty())
	{
		KeyboardEvent e = m_keyboard.ReadBuffer();
		unsigned char keycode = e.GetKeyCode();

		// close window on ESC
		if (keycode == VK_ESCAPE)
			PostMessage(m_window.GetHWND(), WM_CLOSE, NULL, NULL);
	}
}

void Engine::Render()
{
	m_renderer.Render();
}

LRESULT Engine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);

		// if key auto repeat is on, just add the keycode to the key buffer
		// if not, check if the key was held down and if not, add to key buffer
		if (m_keyboard.IsAutoRepeat())
			m_keyboard.KeyDown(keycode);
		else
		{
			const bool wasPressed = lParam & (1 << 30);
			if (!wasPressed)
				m_keyboard.KeyDown(keycode);
		}

		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		m_keyboard.KeyReleased(keycode);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}