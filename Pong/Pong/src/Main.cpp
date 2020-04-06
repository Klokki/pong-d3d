#include "pch.h"
#include "Engine.hpp"

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPWSTR /*lpCmdLine*/, // unreferenced parameters (hPrevInstance, lpCmdLine, nCmdShow)
	_In_ int /*nCmdShow*/)
{
	Engine engine(hInstance, "Pong", "PongClass1", SCR_WIDTH, SCR_HEIGHT);

	while (engine.ProcessMessages() == true)
	{
		engine.Update();
	}

	return 0;
}