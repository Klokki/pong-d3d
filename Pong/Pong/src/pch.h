#pragma once

// WINDOWS INCLUDES
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <comdef.h>
#include <pathcch.h>
#include <wrl/client.h>

// DIRECTX INCLUDES
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// STL INCLUDES
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <chrono>

#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>

// PROJECT INCLUDES
#include "Utility/Error.hpp"
#include "Utility/Profiler.hpp"