#pragma once

// WINDOWS INCLUDES
#define WIN32_LEAN_AND_MEAN
#include <xaudio2.h> // include xaudio2 first so no problems from windows.h
#include <Windows.h>
#include <comdef.h>
#include <pathcch.h>
#include <wrl/client.h>

// WINDOWS MEDIA FOUNDATION
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

// DIRECTX INCLUDES
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

// STL INCLUDES
#include <string>
#include <array>
#include <queue>
#include <vector>

#include <map>
#include <chrono>
#include <algorithm>
#include <fstream>

// PROJECT INCLUDES
#include "Utility/Error.hpp"
