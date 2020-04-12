#include "pch.h"
#include "Engine.hpp"

using ms = std::chrono::duration<float, std::milli>;

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPWSTR /*lpCmdLine*/, // unreferenced parameters (hPrevInstance, lpCmdLine, nCmdShow)
	_In_ int /*nCmdShow*/)
{
	Engine engine(hInstance, "Pong", "PongClass1", SCR_WIDTH, SCR_HEIGHT);

	std::chrono::high_resolution_clock timer;
	auto startTime = timer.now();

	while (engine.ProcessMessages() == true)
	{
		auto currentFrame = timer.now();
		float deltaTime = std::chrono::duration_cast<ms>(currentFrame - startTime).count();
		startTime = currentFrame;

		engine.Update(deltaTime);
		engine.Render();
	}

	return 0;
}