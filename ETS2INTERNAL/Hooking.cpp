#include "pch.h"
#include "Hooking.h"
static IDXGISwapChain* pSwapChain = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

Hooking::Hooking(void* SwapChain[18]) {
    g_hooking = this;

    writeToConsole("Starting Hooking...");
    m_swapchain_hook.start("Present", SwapChain[8], &Hooks::swapchain_present);
    m_swapchain_hook.enable();
    m_resizeBuffers_hook.start("ResizeBuffer", SwapChain[13], &Hooks::swapchain_resizebuffers);
    m_resizeBuffers_hook.enable();
    m_set_cursor_pos_hook.start("SetCusorPosition", GetProcAddress(GetModuleHandleA("user32.dll"), "SetCursorPos"),&Hooks::set_cursor_pos);
    m_set_cursor_pos_hook.enable();
    writeToConsole("Finished Hooking...");
}


HRESULT __fastcall Hooks::swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
{
    if (!g_bInitialised) {
        writeToConsole("Present hook begining initalise!");

        if (FAILED(GetDeviceAndCtxFromSwapchain(this_, &pDevice, &pContext)))
            return g_hooking->m_swapchain_hook.get_original<decltype(&Hooks::swapchain_present)>()(this_, sync_interval, flags);

        pSwapChain = this_;
        ImGui::CreateContext();

        HWND hWnd = FindWindowW(L"prism3d", nullptr);
        if (!hWnd)
            writeToConsole("Cant find window!",true);
        else
            g_hooking->m_hwndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)Hooks::hWndProc);

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pContext);
        ImGui::GetIO().ImeWindowHandle = hWnd;
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
        GUI::drawMenu();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return g_hooking->m_swapchain_hook.get_original<decltype(&Hooks::swapchain_present)>()(this_, sync_interval, flags);
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

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Hooks::hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYUP && wParam == VK_INSERT)
    {
        g_ShowMenu = !g_ShowMenu;
    }

    if (g_ShowMenu)
    {
        //ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam); [Not working correctly]
        return true;
    }

    return CallWindowProc(g_hooking->m_hwndProc, hwnd, uMsg, wParam, lParam);
}

BOOL Hooks::set_cursor_pos(int x, int y) {
    
    if (g_ShowMenu) {
        return false;
    }

    return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&Hooks::set_cursor_pos)>()(x,y);
}

Hooking::~Hooking() 
{
    //TODO DESTROY HOOKS
    writeToConsole("Destroying hooking!");
}

minhook_keepalive::minhook_keepalive()
{
    MH_Initialize();
}

minhook_keepalive::~minhook_keepalive()
{
    MH_Uninitialize();
}
