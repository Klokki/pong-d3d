#include "pch.h"
#include "Keyboard.hpp"

KeyboardEvent Keyboard::ReadBuffer()
{
	// if key buffer is empty, return empty keyboard event
	if (m_keyBuffer.empty())
		return KeyboardEvent();
	else
	{
		KeyboardEvent event = m_keyBuffer.front();
		m_keyBuffer.pop();
		return event;
	}
}

void Keyboard::KeyDown(const unsigned char key)
{
	m_keys[key] = true;
	m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Type::PRESS, key));
}

void Keyboard::KeyReleased(const unsigned char key)
{
	m_keys[key] = false;
	m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Type::RELEASE, key));
}
