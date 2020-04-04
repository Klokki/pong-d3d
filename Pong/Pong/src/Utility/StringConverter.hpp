#pragma once

#include "pch.h"

// StringConverter.hpp
// Very simple (not safe) converter from std::string to std::wstring

class StringConverter
{
public:
	static std::wstring to_wstring(std::string str)
	{
		std::wstring wide_string(str.begin(), str.end());
		return wide_string;
	}
};