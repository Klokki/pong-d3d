#pragma once

#include "Window/Window.hpp"
#include "Input/Keyboard.hpp"
#include "Graphics/Renderer.hpp"

class Engine
{
public:
	Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);

	bool ProcessMessages()
	{
		return m_window.ProcessMessages();
	}

	void Update();
	void Render();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Window m_window;
	Keyboard m_keyboard;
	Renderer m_renderer;
	//Renderer m_renderer;
};