#pragma once

#include "pch.h"

// OutputPath.hpp
// Returns program output (bin) path

class OutputPath
{
public:
	static std::wstring get()
	{
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(NULL, path, MAX_PATH);
		PathCchRemoveFileSpec(path, sizeof(path));

		return (std::wstring)path;
	}
};