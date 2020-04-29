#pragma once

#include "pch.h"
#include "StringConverter.hpp"

// Error.hpp
// Spawns a Windows message box with an error message

class Error
{
public:
	static void Message(std::string message)
	{
		std::string error_message = "Error: " + message;
		MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	static void Message(HRESULT hr, std::string message)
	{
		_com_error error(hr);
		std::wstring error_message = L"Error: " + StringConverter::to_wstring(message) + L"\n" + error.ErrorMessage();
		MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	static void Message(HRESULT hr, std::wstring message)
	{
		_com_error error(hr);
		std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
		MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
};
