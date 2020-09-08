#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>

#include "../pastascat_sdk/hash.h"

// Assert macro
#define assert(x) \
if (!x) \
{ \
    puts("Failed!"); \
    return 1; \
}

int main()
{
    HANDLE hCSGO     = nullptr; // Open process handle to CS:GO
    void*  fnLoadLib = nullptr; // Pointer to the LoadLibraryA function

    puts("pastascat loader for CS:GO");

    // Obtain process handle
    {
        // Create a process snapshot
        printf("Creating process snapshot... ");
        HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        assert(hProcSnap);

        // Parse the process snapshot
        printf("0x%p\nLocating CS:GO... ", hProcSnap);
        PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
        while (Process32Next(hProcSnap, &pe32))
        {
            if (pc::utils::cx_fnv(pe32.szExeFile, sizeof("csgo.exe") - 1) == pc::utils::cx_fnv("csgo.exe"))
            {
                printf(" Opening handle... ");
                hCSGO = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
                break;
            }
        }

        assert(hCSGO);

        puts("ok!");
    }

    // Obtain pointer to LoadLibraryW
    {
        printf("Obtaining LoadLibraryW... ");
        HMODULE hmKernel32 = GetModuleHandle(L"Kernel32");
        if (!hmKernel32
        ||  !(fnLoadLib = GetProcAddress(hmKernel32, "LoadLibraryW"))
        ) {
            puts("Failed!");
            return 1;
        }

        puts("ok!");
    }
    
    // Load the DLL
    {
        wchar_t szDLLPath[MAX_PATH] = { '\0' };
        printf("Loading full path: ");
        size_t nPathSize = GetFullPathName(L"pastascat_dll.dll", MAX_PATH - 1, szDLLPath, nullptr) * sizeof(TCHAR); // Obtain the full path (MAX_PATH - 1 just for null terminator sanity)
        assert(nPathSize);
        wprintf(L"%ls", szDLLPath);
        
        // Allocate memory in CS:GO to hold the path string buffer
        printf("Allocating memory... ");
        HANDLE pAllocStrBuff = VirtualAllocEx(hCSGO, nullptr, nPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        assert(pAllocStrBuff);

        // Write the string
        printf("0x%p\nWriting... ", pAllocStrBuff);
        assert(WriteProcessMemory(hCSGO, pAllocStrBuff, szDLLPath, nPathSize, nullptr));
        puts("ok!");
        
        // Execute LoadLibraryW
        printf("Creating remote thread... ");
        HANDLE hCRT = CreateRemoteThread(hCSGO, nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(fnLoadLib), pAllocStrBuff, NULL, nullptr);
        assert(hCRT);
        printf("[0x%p] Waiting... ", hCRT);
        WaitForSingleObject(hCRT, INFINITE);
        puts("ok!");

        // Cleanup
        printf("Cleaning up... ");
        if (!VirtualFreeEx(hCSGO, pAllocStrBuff, NULL, MEM_RELEASE))
        {
            printf("FAILED!\nWarning: Memory leak at: 0x%p with size of %d bytes", pAllocStrBuff, nPathSize);
            return 1;
        }
        puts("ok!");
    }

    return 0;
}