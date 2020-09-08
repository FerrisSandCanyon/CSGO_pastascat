#include "./hk.h"
#include "../../pastascat_sdk/MinHook.h"

namespace pc
{
	namespace hooked
	{
		LRESULT __stdcall WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
		{
			// Listen for toggle mode
			if (uMsg == WM_KEYUP && wParam == VK_INSERT)
			{
				pc::cheat::bToggleMode ^= true;
				return TRUE;
			}

			// Call original when not in toggle mode
			if (!pc::cheat::bToggleMode)
				return pc::ohk::wndproc(hwnd, uMsg, wParam, lParam);

			// Only handle WM_KEYUP messages
			if (uMsg != WM_KEYUP)
				return TRUE;

			// Handle the messages when on toggle mode
			switch (wParam)
			{
				case VK_END: // Unload
				{
					void (*exitthread)(LPVOID) = [](LPVOID arg) -> void
					{
						// Restore hooks
						SetWindowLongPtr(pc::global::hwndCSGO, GWLP_WNDPROC, reinterpret_cast<LONG>(pc::ohk::wndproc));
						MH_DisableHook(MH_ALL_HOOKS);
						MH_Uninitialize();

						// Free the library
						FreeLibraryAndExitThread(pc::global::hmodDLL, 0);
					};

					CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exitthread), nullptr, NULL, nullptr);
					break;
				}
			}

			return TRUE;
		}
	}
}