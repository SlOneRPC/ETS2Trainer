#include "pch.h"
#include "Hooking.h"
#include "Menu.h"
#include "Features.h"
bool g_unload = false;

DWORD WINAPI MainThread(HMODULE module) 
{
    AttachConsole();

    auto features_instance = std::make_unique<Features>();
    auto menu_instance = std::make_unique<Menu>();
    auto hooking_instance = std::make_unique<Hooking>();

    LogInfo("Setup Complete :)");
    while (!g_unload) 
    {
        if (GetAsyncKeyState(VK_END) & 0x8000)
            g_unload = true;

        std::this_thread::sleep_for(10ms);
    }

    g_hooking->disable();
    LogInfo("Freeing library and exiting...");
    CloseConsole();
    FreeLibraryAndExitThread(module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

