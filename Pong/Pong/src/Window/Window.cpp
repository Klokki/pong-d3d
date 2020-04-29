#include "pch.h"
#include "Window.hpp"
#include "Engine.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(Engine* eng, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	:
	m_hInstance(hInstance),
	m_width(width),
	m_height(height),
	m_window_title(window_title),
	m_window_class(window_class)
{
	m_window_title_wide = StringConverter::to_wstring(m_window_title);
	m_window_class_wide = StringConverter::to_wstring(m_window_class);

	// register window class
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	HRESULT hr;
	if (FAILED(hr = RegisterClassEx(&wc)))
		Error::Message(hr, "Failed to register window class");

	// Center window on screen and adjust the window rectangle
	LONG posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	LONG posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	RECT wr = { posX, posY, posX + m_width, posY + m_height };
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	m_handle = CreateWindowEx
	(
		WS_EX_APPWINDOW, // Forces a top-level window onto the taskbar when the window is visible
		m_window_class_wide.c_str(),
		m_window_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left, // window X position
		wr.top, // window Y position
		wr.right - wr.left, // window width
		wr.bottom - wr.top, // window height
		NULL,
		NULL,
		m_hInstance,
		eng
	);

	if (m_handle != NULL)
	{
		// show and set window as foreground
		ShowWindow(m_handle, SW_SHOW);
		SetForegroundWindow(m_handle);
		SetFocus(m_handle);
	}
	else
		Error::Message(GetLastError(), "CreateWindowEX failed for window " + m_window_title);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// if window was closed, message process loop destroys it
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_handle))
		{
			m_handle = NULL;
			UnregisterClass(this->m_window_class_wide.c_str(), this->m_hInstance);
			return false;
		}
	}

	return true;
}

Window::~Window()
{
	if (m_handle != NULL)
	{
		UnregisterClass(this->m_window_class_wide.c_str(), this->m_hInstance);
		DestroyWindow(m_handle);
	}
}
