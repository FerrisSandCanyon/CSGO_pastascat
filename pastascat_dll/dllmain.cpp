#include <Windows.h>

#if _DEBUG
    #pragma comment(lib, "../pastascat_sdk/MinHook_lib/libMinHook-x86-v141-mdd.lib")
#else
    #pragma comment(lib, "../pastascat_sdk/MinHook_lib/libMinHook-x86-v141-mtd.lib")
#endif

#include "../pastascat_sdk/MinHook.h"
#include "../pastascat_sdk/xorstr.h"
#include "./globals.h"
#include "./hooks/hk.h"

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
    void* iGameUI = nullptr; // Interface pointer to GameUI011 (only used here at init)

    #pragma region Initial setup
    // Obtain CS:GO's window handle
    {
        int nTimeToWait = 30; // Amount of time in seconds to wait for CS:GO's window
        
        // Obtain CS:GO's window
        while ( !(pc::global::hwndCSGO = FindWindowW(xorstr(L"Valve001"), xorstr(L"Counter-Strike: Global Offensive"))) )
        {
            Sleep(1000);
            if (--nTimeToWait <= 0)
            {
                MessageBoxW(pc::global::hwndCSGO, xorstr(L"Timed out waiting for CS:GO's waindow"), xorstr(L"pastascat"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return 1;
            }
        }
    }

    // Wait for the "last" dll to be loaded
    {
        int nTimeToWait = 30;// Amount of time in seconds to wait for serverbrowser.dll

        // Try to obtain serverbrowser.dll module which should indicate that it's loaded
        while (!GetModuleHandleW(xorstr(L"serverbrowser.dll")))
        {
            Sleep(1000);
            if (--nTimeToWait <= 0)
            {
                MessageBoxW(pc::global::hwndCSGO, xorstr(L"Timed out waiting for serverbrowser.dll"), xorstr(L"pastascat"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return 1;
            }
        }
    }
    #pragma endregion

    #pragma region Interface setup
    {
        // Create interface helper function
        void* (*GetInterface)(const wchar_t* szModule, const char* szName) = [](const wchar_t* szModule, const char* szIName) -> void*
        {
            // Obtain module
            HMODULE hMod = GetModuleHandleW(szModule);
            if (!hMod)
            {
                MessageBoxW(pc::global::hwndCSGO, szModule, xorstr(L"Failed to locate module"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return nullptr;
            }

            // Obtain CreateInterface
            void* (__cdecl * CreateInterface)(const char* _szIName, int* retCode) = reinterpret_cast<void* (__cdecl*)(const char*, int*)>(GetProcAddress(hMod, xorstr("CreateInterface")));
            if (!CreateInterface)
            {
                MessageBoxW(pc::global::hwndCSGO, szModule, xorstr(L"CreateInterface cant be exported"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return nullptr;
            }

            // Call CreateInterface and obtain a pointer to the specified interface
            void* pInterface = CreateInterface(szIName, nullptr);
            if (!pInterface)
            {
                MessageBoxA(pc::global::hwndCSGO, szIName, xorstr("Interface cant be exported"), 0);
                FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
                return nullptr;
            }

            return pInterface;
        };

        iGameUI = GetInterface(xorstr(L"client"), xorstr("GameUI011"));
        pc::iface::Surface = reinterpret_cast<ISurface*>(GetInterface(xorstr(L"vguimatsurface"), xorstr("VGUI_Surface031")));
        pc::iface::Panel = GetInterface(xorstr(L"vgui2"), xorstr("VGUI_Panel009"));
        pc::iface::Engine = GetInterface(xorstr(L"engine"), xorstr("VEngineClient014"));
    }
    #pragma endregion

    #pragma region Hook Setup
    // Hook WindowProcedure
    if ( !(pc::ohk::wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(pc::global::hwndCSGO, GWLP_WNDPROC, reinterpret_cast<LONG>(pc::hooked::WindowProc)))) )
    {
        MessageBox(pc::global::hwndCSGO, xorstr(L"Failed to hook the windows procedure callback"), xorstr(L"pastascat"), 0);
        FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
        return 1;
    }

    // Initialize MinHook
    if (MH_Initialize() != MH_OK)
    {
        MessageBox(pc::global::hwndCSGO, xorstr(L"Failed to initialize MinHook"), xorstr(L"pastascat"), 0);
        FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
        return 1;
    }

    // Hook PaintTraverse
    if (MH_CreateHook(reinterpret_cast<void***>(pc::iface::Panel)[0][41], pc::hooked::PaintTraverse, reinterpret_cast<void**>(&pc::ohk::PaintTraverse)))
    {
        MessageBox(pc::global::hwndCSGO, xorstr(L"Failed to hook PaintTraverse"), xorstr(L"pastascat"), 0);
        FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
        return 1;
    }

    // Hook ClientCMD
    if (MH_CreateHook(reinterpret_cast<void***>(pc::iface::Engine)[0][7], pc::hooked::ClientCMD, reinterpret_cast<void**>(&pc::ohk::ClientCMD)))
    {
        MessageBox(pc::global::hwndCSGO, xorstr(L"Failed to hook ClientCMD"), xorstr(L"pastascat"), 0);
        FreeLibraryAndExitThread(pc::global::hmodDLL, 1);
        return 1;
    }

    // Register the hooks
    MH_EnableHook(MH_ALL_HOOKS);
    #pragma endregion
    
    #pragma region Finalize
    // Create font
    pc::cheat::font = reinterpret_cast<unsigned long(*)(void)>(reinterpret_cast<void***>(pc::iface::Surface)[0][71])();
    pc::iface::Surface->SetFont(pc::cheat::font, "Tahoma", 12, 500, 0, 0, FontFlags::OUTLINE);

    #pragma endregion

    reinterpret_cast<void(__thiscall*)(void*,const char*, const char*, bool, bool, const char*, const char*, const char*, const char*, const char*)>(reinterpret_cast<void***>(iGameUI)[0][20])(iGameUI,
    xorstr("pastascat practice cheat"),
    xorstr(
    "Insert - Toggle Mode\n"
    "Toggle Mode Keybinds:\n"
    "TAB - Player list\n"
    "END - Unload\n"
    "QWERTY... - Option position corresponds to option toggle\n"
    "[ - Previous clantag\n"
    "] - Next clantag\n"
    "Custom commands:\n"
    "!clantag - Sets the clantag"
    ), true, false, nullptr, nullptr, nullptr, nullptr, nullptr);

    return 0;
}