#pragma once

#include "Window/Window.hpp"
#include "Input/Keyboard.hpp"

class Engine
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	{
		return this->m_window.Initialize(this, hInstance, window_title, window_class, width, height);
	}

	bool ProcessMessages()
	{
		return this->m_window.ProcessMessages();
	}

	void Update();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Window m_window;
	Keyboard m_keyboard;
};