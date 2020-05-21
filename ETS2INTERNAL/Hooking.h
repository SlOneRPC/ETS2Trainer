#pragma once

struct Hooks {
	static HRESULT __fastcall swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags);
	static HRESULT swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
	static LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL set_cursor_pos(int x, int y);
};

struct minhook_keepalive
{
	minhook_keepalive();
	~minhook_keepalive();
};

class Hooking
{
	friend Hooks;
public:
	explicit Hooking(void* SwapChain[18]);
	~Hooking();

private:
	minhook_keepalive m_minhook_keepalive;
	detour_hook m_swapchain_hook;
	detour_hook m_resizeBuffers_hook;
	detour_hook m_set_cursor_pos_hook;
	WNDPROC m_hwndProc;
};

inline bool g_bInitialised = false;
inline bool g_ShowMenu = false;
inline Hooking* g_hooking{};
