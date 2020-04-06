#pragma once

#include "KeyboardEvent.hpp"

// Keyboard.hpp
// Contains and modifies the state of the keyboard (whether a specific key is pressed, etc)

class Keyboard
{
public:
	Keyboard(){}
	bool KeyPressed(const unsigned char keycode) const { return m_keys[keycode]; }
	bool KeyBufferEmpty() const { return m_keyBuffer.empty(); }
	KeyboardEvent ReadBuffer();
	void KeyDown(const unsigned char key);
	void KeyReleased(const unsigned char key);
	void SetAutoRepeat(bool value) { m_autoRepeat = value; }
	bool IsAutoRepeat() const { return m_autoRepeat; }
private:
	bool m_autoRepeat = false;
	bool m_keys[256] = { false }; // initialize all keys to false
	std::queue<KeyboardEvent> m_keyBuffer;
};