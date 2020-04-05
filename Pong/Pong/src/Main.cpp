#include "pch.h"

#include "Engine.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPWSTR /*lpCmdLine*/, // unreferenced parameters (hPrevInstance, lpCmdLine, nCmdShow)
	_In_ int /*nCmdShow*/)
{
	Engine engine;
	engine.Initialize(hInstance, "Pong", "PongClass1", 800, 600);

	while (engine.ProcessMessages() == true)
	{
	}

	return 0;
}