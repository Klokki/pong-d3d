#pragma once

#include "Window/Window.hpp"
#include "Input/Keyboard.hpp"

class Engine
{
public:
	Engine(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);

	bool ProcessMessages()
	{
		return m_window.ProcessMessages();
	}

	void Update();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Window m_window;
	Keyboard m_keyboard;
};