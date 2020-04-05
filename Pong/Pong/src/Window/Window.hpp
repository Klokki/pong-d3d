#pragma once

#include "Utility/Error.hpp"

// Window.hpp
// Representation of a single Windows window

class Engine;

class Window
{
public:
	bool Initialize(Engine* eng, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	~Window();
private:
	void registerWindowClass();

	HWND m_handle						= NULL;
	HINSTANCE m_hInstance				= NULL;
	std::string m_window_title			= "";
	std::wstring m_window_title_wide	= L"";
	std::string m_window_class			= "";
	std::wstring m_window_class_wide	= L"";

	LONG m_width		= 0;
	LONG m_height		= 0;
};