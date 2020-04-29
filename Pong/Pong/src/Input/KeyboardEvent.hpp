#pragma once

// KeyboardEvent.hpp
// A single keyboard event, whether keydown or keyup

class KeyboardEvent
{
public:
	enum class Type
	{
		KEYDOWN,
		KEYUP,
		INVALID
	};

	KeyboardEvent()
	{
		m_type = Type::INVALID;
		m_key = 0u;
	}

	KeyboardEvent(const Type type, const unsigned char key)
	{
		m_type = type;
		m_key = key;
	}

	bool KeyDown() const { return m_type == Type::KEYDOWN; }
	bool KeyUp() const { return m_type == Type::KEYUP; }
	unsigned char GetKeyCode() const { return m_key; }
private:
	Type m_type;
	unsigned char m_key;
};
