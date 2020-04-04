#include "pch.h"
#include "Window.hpp"

bool Window::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance			= hInstance;
	this->width				= width;
	this->height			= height;
	this->window_title		= window_title;
	this->window_title_wide = StringConverter::to_wstring(this->window_title);
	this->window_class		= window_class;
	this->window_class_wide	= StringConverter::to_wstring(this->window_class);

	// center window on screen
	posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	// adjust window size according to width and height (of client)
	RECT wr = { 0, 0, this->width, this->height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	this->registerWindowClass();

	this->handle = CreateWindowEx
	(
		WS_EX_APPWINDOW, // Forces a top-level window onto the taskbar when the window is visible
		this->window_class_wide.c_str(),
		this->window_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, // window X position
		posY, // window Y position
		wr.right - wr.left, // window width
		wr.bottom - wr.top, // window height
		NULL,
		NULL,
		this->hInstance,
		nullptr
	);

	if (this->handle == NULL)
	{
		Error::Message(GetLastError(), "CreateWindowEX failed for window " + this->window_title);
		return false;
	}

	// show and set window as foreground
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// if window was closed, message process loop destroys it
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

Window::~Window()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

void Window::registerWindowClass()
{
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}
