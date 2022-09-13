#include "pch.h"

struct Hooks {
	static HRESULT __fastcall swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags);
	static HRESULT swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
	static LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK hWndProcInternal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL set_cursor_pos(int x, int y);
	static HWND setWndLongPtrHk(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
	static BOOL GetCursorPos(LPPOINT lpPoint);
	static LRESULT CallWindowProcWHk(WNDPROC lpPrevWndFunc,
		HWND hWnd,
		UINT Msg,
		WPARAM wParam,
		LPARAM lParam);
	static DWORD WINAPI XInputGetState__Hook(DWORD dwUserIndex, XINPUT_STATE* pState);
	static HRESULT __stdcall DirectInput8Create__Hook(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
	static int GetCursorInfoHk(PCURSORINFO pci);
};

struct minhook_keepalive
{
	minhook_keepalive();
	~minhook_keepalive();
};

class Hooking {
	friend Hooks;
public:
	explicit Hooking();
	~Hooking();

	void disable();
	LRESULT KeyBoardProc(int code, WPARAM wParam, LPARAM lParam);
	LRESULT MouseProc(int code, WPARAM wParam, LPARAM lParam);
private:
	minhook_keepalive m_minhook_keepalive;
	detour_hook m_swapchain_hook;
	detour_hook m_resizeBuffers_hook;
	detour_hook m_set_cursor_pos_hook;

	detour_hook m_xinput;
	detour_hook m_DirectInput8;

	WNDPROC m_hwndProc;
	HWND window;
};

inline bool g_bInitialised = false;
inline bool g_ShowMenu = false;
inline Hooking* g_hooking;