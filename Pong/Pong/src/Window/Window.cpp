#include "pch.h"
#include "Window.hpp"
#include "Engine.hpp"

void Window::Initialize(Engine* eng, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	m_hInstance = hInstance;
	m_width = width;
	m_height = height;
	m_window_title = window_title;
	m_window_title_wide = StringConverter::to_wstring(m_window_title);
	m_window_class = window_class;
	m_window_class_wide = StringConverter::to_wstring(m_window_class);

	// center window on screen
	unsigned int posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	unsigned int posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	// adjust window size according to width and height (of client)
	RECT wr = { 0, 0, this->m_width, this->m_height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	this->registerWindowClass();

	m_handle = CreateWindowEx
	(
		WS_EX_APPWINDOW, // Forces a top-level window onto the taskbar when the window is visible
		this->m_window_class_wide.c_str(),
		this->m_window_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, // window X position
		posY, // window Y position
		wr.right - wr.left, // window width
		wr.bottom - wr.top, // window height
		NULL,
		NULL,
		this->m_hInstance,
		eng
	);

	if (m_handle == NULL)
	{
		Error::Message(GetLastError(), "CreateWindowEX failed for window " + m_window_title);
		exit(EXIT_FAILURE);
	}

	// show and set window as foreground
	ShowWindow(m_handle, SW_SHOW);
	SetForegroundWindow(m_handle);
	SetFocus(m_handle);
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static Engine* p_engine = nullptr;

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		p_engine = reinterpret_cast<Engine*>(pCreate->lpCreateParams);
	}break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}

	default:
		if(!p_engine)
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return p_engine->WindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::registerWindowClass()
{
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->m_window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}