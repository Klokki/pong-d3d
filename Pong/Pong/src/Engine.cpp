#include "pch.h"
#include "Engine.hpp"
#include "Utility/OutputPath.hpp"

Engine::Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	:
	m_window(this, hInstance, window_title, window_class, width, height),
	m_renderer(m_window.GetHWND(), width, height),
	m_audio(),
	m_game(width, height, &m_audio)
{
	std::wstring outputPath = OutputPath::get();

	m_audio.LoadFile(outputPath + L"\\audio\\bleep1.wav", "bleep1", *m_bleep1);
	m_audio.LoadFile(outputPath + L"\\audio\\bleep2.wav", "bleep2", *m_bleep2);
	m_audio.LoadFile(outputPath + L"\\audio\\bleep3.wav", "bleep3", *m_bleep3);
}

Engine::~Engine()
{
}

void Engine::Update(float delta)
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

		// F2 to switch vsync on/off (default on)
		if (keycode == VK_F2 && e.KeyUp() == true)
			m_vsync = !m_vsync;

		m_game.HandleInput(keycode);
	}

	m_game.Update(delta);
}

void Engine::Render()
{
	m_renderer.BeginRender();
	m_game.Render(m_renderer);
	m_renderer.EndRender(m_vsync);
}
