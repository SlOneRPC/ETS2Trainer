#include "../../pch.h"
#include "Utils.h"
FILE* fStream;
void AttachConsole()
{
    if (AttachConsole(GetCurrentProcessId()) == false)
        AllocConsole();

    freopen_s(&fStream, "CONOUT$", "w", stdout);
    SetConsoleTitle(L"ETS2 Menu");
}

void CloseConsole() 
{
    fclose(fStream);
    FreeConsole();
}
