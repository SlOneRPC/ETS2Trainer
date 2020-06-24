#include "pch.h"
#include "Hooking.h"
#include "Features.h"
bool g_unload = false;
void handleMouseInput() 
{
    if (g_ShowMenu && (GetKeyState(VK_LBUTTON) & 0x100) != 0) {
        ImGui::GetIO().MouseDown[0] = true;
    }
    else {
        ImGui::GetIO().MouseDown[0] = false;
    }
}

BOOL APIENTRY MainThread(HMODULE pHandle)
{
    g_hmodule = pHandle;
    g_cheat_thread = CreateThread(nullptr, 0, [](PVOID)->DWORD
    {
        //Attach a console
        if (AttachConsole(GetCurrentProcessId()) == false)
            AllocConsole();
        FILE* fStream;
        freopen_s(&fStream,"CONOUT$", "w", stdout);
        SetConsoleTitle(L"ETS2 Menu");

        void* SwapChain[18];
        if (GetD3D11SwapchainDeviceContext(SwapChain, sizeof(SwapChain))) {
            //start hooking by creating a hooking instance
            auto hooking_instance = std::make_unique<Hooking>(SwapChain);

            while (!g_unload) {
                handleMouseInput();//cant handle mouse input with WndProc for some reason, this is an alternative
                std::this_thread::sleep_for(10ms);
                if (GetAsyncKeyState(VK_END) & 0x8000)
                    g_unload = true;
                Features::runLoop();
            }
            g_hooking->disable();
            hooking_instance.reset();
        }
        writeToConsole("Closing handle and exiting...");
        if(!FreeConsole())
            writeToConsole("Cant close console!",true);

        CloseHandle(g_cheat_thread);
        FreeLibraryAndExitThread(g_hmodule, 0);
    }, nullptr, 0, &g_cheat_thread_id);

    return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        MainThread(hModule);
        //CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

