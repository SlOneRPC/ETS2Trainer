#include "Hooking.h"
#include "SDK/Dx11/Swapchain.h"
#include "IDirectInput8Proxy.h"
#include "Features.h"

static IDXGISwapChain* pSwapChain = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

Hooking::Hooking()
{
	g_hooking = this;

    LogInfo("Starting Hooking...");
    void* SwapChain[18];
    if (GetD3D11SwapchainDeviceContext(SwapChain, sizeof(SwapChain), L"prism3d")) 
    {
        m_swapchain_hook.start("Present", SwapChain[8], &Hooks::swapchain_present);
        m_swapchain_hook.enable();
        m_resizeBuffers_hook.start("ResizeBuffer", SwapChain[13], &Hooks::swapchain_resizebuffers);
        m_resizeBuffers_hook.enable();

        // Extra hooks
        m_set_cursor_pos_hook.start("SetCusorPosition", GetProcAddress(GetModuleHandleA("user32.dll"), "SetCursorPos"), &Hooks::set_cursor_pos);
        m_set_cursor_pos_hook.enable();
        m_xinput.start("XInputGetState", GetProcAddress(GetModuleHandleA("xinput1_3.dll"), "XInputGetState"), &Hooks::XInputGetState__Hook);
        m_xinput.enable();
        m_DirectInput8.start("DirectInput8Create", GetProcAddress(GetModuleHandleA("dinput8.dll"), "DirectInput8Create"), &Hooks::DirectInput8Create__Hook);
        m_DirectInput8.enable();

        m_peekMessage.start("PeekMessageW", GetProcAddress(GetModuleHandleA("user32.dll"), "PeekMessageW"), &Hooks::PeekMessageWHk);
        m_peekMessage.enable();
    }
    LogInfo("Finished Hooking...");
}

HRESULT __fastcall Hooks::swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
{
    if (!g_bInitialised) {
        LogInfo("Present hook begining initalise!");

        if (FAILED(GetDeviceAndCtxFromSwapchain(this_, &pDevice, &pContext)))
            return g_hooking->m_swapchain_hook.get_original<decltype(&Hooks::swapchain_present)>()(this_, sync_interval, flags);

        pSwapChain = this_;
        ImGui::CreateContext();

        g_hooking->window = GetWindowMeme();
        if (!g_hooking->window)
            LogError("Cant find window!");
        else
            g_hooking->m_hwndProc = (WNDPROC)SetWindowLongPtr(g_hooking->window, GWLP_WNDPROC, (LONG_PTR)Hooks::hWndProc);

        ImGui_ImplWin32_Init(g_hooking->window);
        ImGui_ImplDX11_Init(pDevice, pContext);
        ImGui::GetIO().ImeWindowHandle = g_hooking->window;
        ImGui::GetIO().Fonts->AddFontDefault();

        g_bInitialised = true;
    }

    if (g_ShowMenu)
        ImGui::GetIO().MouseDrawCursor = true;
    else
        ImGui::GetIO().MouseDrawCursor = false;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    if (g_ShowMenu)
    {
        g_Menu->OnRender();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return g_hooking->m_swapchain_hook.get_original<decltype(&Hooks::swapchain_present)>()(this_, sync_interval, flags);
}

DWORD WINAPI Hooks::XInputGetState__Hook(DWORD dwUserIndex, XINPUT_STATE* pState)
{
    DWORD dwResult = g_hooking->m_xinput.get_original<decltype(&Hooks::XInputGetState__Hook)>()(dwUserIndex, pState);

    if (g_ShowMenu)
    {
        memset(pState, 0, sizeof(XINPUT_STATE));
    }

    return dwResult;
}

HRESULT WINAPI Hooks::DirectInput8Create__Hook(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    HRESULT hResult = g_hooking->m_xinput.get_original<decltype(&Hooks::DirectInput8Create__Hook)>()(hInst, dwVersion, riidltf, ppvOut, punkOuter);

    if (SUCCEEDED(hResult))
    {
        IDirectInput8* pInput = (IDirectInput8*)*ppvOut;
        *ppvOut = new IDirectInput8Proxy(pInput);
    }

    return hResult;
}

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL __stdcall Hooks::PeekMessageWHk(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
    if (g_ShowMenu && lpMsg->message == WM_INPUT)
    {
        return 0;
    }

    return g_hooking->m_peekMessage.get_original<decltype(&Hooks::PeekMessageWHk)>()(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

HRESULT Hooks::swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
{
    ImGui_ImplDX11_InvalidateDeviceObjects();

    auto result = g_hooking->m_resizeBuffers_hook.get_original<decltype(&Hooks::swapchain_resizebuffers)>()
        (this_, buffer_count, width, height, new_format, swapchain_flags);

    if (SUCCEEDED(result))
    {
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    return result;
}

LRESULT CALLBACK Hooks::hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYUP)
    {
        if (wParam == VK_INSERT)
            g_ShowMenu = !g_ShowMenu;
        else if (wParam == VK_F9 && g_Options.teleportHotkey)
            g_Features->TeleportToCameraCoords();
    }

    if (g_ShowMenu)
    {
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
        return true;
    }

    return CallWindowProc(g_hooking->m_hwndProc, hwnd, uMsg, wParam, lParam);
}

BOOL Hooks::set_cursor_pos(int x, int y) {

    if (g_ShowMenu) {
        return false;
    }

    return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&Hooks::set_cursor_pos)>()(x, y);
}

void Hooking::disable() {
    m_swapchain_hook.disable();
    m_resizeBuffers_hook.disable();
    m_set_cursor_pos_hook.disable();
    m_xinput.disable();
    m_DirectInput8.disable();
    m_peekMessage.disable();

    if (g_hooking->window)
        SetWindowLongPtrW(g_hooking->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_hwndProc));
    LogInfo("Unhooked all!");
}

Hooking::~Hooking()
{
    g_hooking = nullptr;
    LogInfo("Destroying hooking!");
}

minhook_keepalive::minhook_keepalive()
{
    MH_Initialize();
}

minhook_keepalive::~minhook_keepalive()
{
    MH_Uninitialize();
}