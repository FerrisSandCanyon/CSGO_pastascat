#include "pch.h"

// Function prototypes
DWORD __stdcall __init_thread(LPVOID lpThreadParameter);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        pc::global::hmodDLL = hModule;
        CreateThread(nullptr, NULL, __init_thread, nullptr, NULL, nullptr);
    }
    
    return TRUE;
}

/// <summary>
/// Initializing thread
/// </summary>
/// <param name="lpThreadParameter">Unused, for compliance to LPTHREAD_START_ROUTINE</param>
/// <returns>None</returns>
DWORD __stdcall __init_thread(LPVOID lpThreadParameter)
{
    // Obtain CS:GO's window handle
    {
        int nTimeToWait = 30; // Amount of time in seconds to wait for CS:GO's window
        
        // Obtain CS:GO's window
        while ( !(pc::global::hwndCSGO = FindWindow(xorstr(L"Valve001"), xorstr(L"Counter-Strike: Global Offensive"))) )
        {
            Sleep(1000);
            if (--nTimeToWait <= 0)
            {
                MessageBox(pc::global::hwndCSGO, xorstr(L"Timed out waiting for CS:GO's waindow"), xorstr(L"pastascat"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return 1;
            }
        }
    }

    // Wait for the "last" dll to be loaded
    {
        int nTimeToWait = 30;// Amount of time in seconds to wait for serverbrowser.dll

        // Try to obtain serverbrowser.dll module which should indicate that it's loaded
        while (!GetModuleHandle(xorstr(L"serverbrowser.dll")))
        {
            Sleep(1000);
            if (--nTimeToWait <= 0)
            {
                MessageBox(pc::global::hwndCSGO, xorstr(L"Timed out waiting for serverbrowser.dll"), xorstr(L"pastascat"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return 1;
            }
        }
    }



    return 0;
}