// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <cstddef>
#include <iostream>
#include <array>

//ImGUI imports
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// DirectX Includes
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

//hooking imports
#include "MinHook.h"
#include "Hooking/Detour.h"

//other
#include "GUI/GUI.h"
#include "Utils.h"
#include "GUI/Render.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

inline HMODULE g_hmodule{};
inline HANDLE g_cheat_thread{};
inline DWORD g_cheat_thread_id{};

#endif //PCH_H
