#pragma once

#include "KeyboardEvent.hpp"

// Keyboard.hpp
// Contains the state of the keyboard (whether a specific key is pressed, etc)

class Keyboard
{
public:
	Keyboard(){}
	bool KeyPressed(const unsigned char keycode) { return m_keys[keycode]; }
	bool KeyBufferEmpty() { return m_keyBuffer.empty(); }
	KeyboardEvent ReadBuffer();
	void KeyDown(const unsigned char key);
	void KeyReleased(const unsigned char key);
	void SetAutoRepeat(bool value) { m_autoRepeat = value; }
	bool IsAutoRepeat() { return m_autoRepeat; }
private:
	bool m_autoRepeat = false;
	bool m_keys[256] = { false }; // initialize all keys to false
	std::queue<KeyboardEvent> m_keyBuffer;
};