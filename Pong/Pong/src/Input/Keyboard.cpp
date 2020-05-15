#include "pch.h"
#include "Keyboard.hpp"

void Keyboard::Update()
{
	m_lastKeyState = m_currentKeyState;

	// update keyboard state
	for (int i = 0; i < 256; ++i)
	{
		// if key is down, set key state to true and add to event queue
		// else if key is not down and was in the previous frame, send keyup
		if (isDown(i))
		{
			m_currentKeyState[i] = true;
			m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Type::KEYDOWN, static_cast<unsigned char>(i)));
		}
		else if (!isDown(i) && m_lastKeyState[i] == true)
		{
			m_currentKeyState[i] = false;
			m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Type::KEYUP, static_cast<unsigned char>(i)));
		}
	}
}

KeyboardEvent Keyboard::ReadBuffer()
{
	// if key buffer is not empty, return event, otherwise return empty KeyboardEvent
	if (!KeyBufferEmpty())
	{
		KeyboardEvent event = m_keyBuffer.front();
		m_keyBuffer.pop();

		return event;
	}

	return KeyboardEvent();
}
