#pragma once

class KeyboardEvent
{
public:
	enum class Type
	{
		PRESS,
		RELEASE,
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

	bool IsPress() const { return m_type == Type::PRESS; }
	bool IsRelease() const { return m_type == Type::RELEASE; }
	bool IsValid() const { return m_type != Type::INVALID; }
	unsigned char GetKeyCode() const { return m_key; }
private:
	Type m_type;
	unsigned char m_key;
};