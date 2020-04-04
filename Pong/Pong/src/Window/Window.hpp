#pragma once

#include "Utility/Error.hpp"

// Window.hpp
// Representation of a single Windows window

class Window
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	~Window();
private:
	void registerWindowClass();

	HWND handle						= NULL;
	HINSTANCE hInstance				= NULL;
	std::string window_title		= "";
	std::wstring window_title_wide	= L"";
	std::string window_class		= "";
	std::wstring window_class_wide	= L"";

	unsigned int width	= 0;
	unsigned int height = 0;
	unsigned int posX	= 0;
	unsigned int posY	= 0;
};