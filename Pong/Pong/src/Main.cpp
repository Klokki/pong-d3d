#include "pch.h"

#include "Window/Window.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window window;
	window.Initialize(hInstance, "Pong", "PongClass1", 800, 600);

	while (window.ProcessMessages() == true)
	{
	}

	return 0;
}