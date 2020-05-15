#pragma once

#include "KeyboardEvent.hpp"

// Keyboard.hpp
// Contains and modifies the state of the keyboard (whether a specific key is pressed, etc)

class Keyboard
{
public:
	Keyboard() {}
	void Update();

	bool KeyBufferEmpty() const { return m_keyBuffer.empty(); }
	KeyboardEvent ReadBuffer();
private:
	bool isDown(int key) const { return (GetAsyncKeyState(key) & 0x8000) ? 1 : 0; };
	std::queue<KeyboardEvent> m_keyBuffer;

	// initialize key arrays to false
	std::array<bool, 256> m_currentKeyState = { false };
	std::array<bool, 256> m_lastKeyState = { false };
};
