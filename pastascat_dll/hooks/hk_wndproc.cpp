#include "./hk.h"

namespace pc
{
	namespace hooked
	{
		LRESULT __stdcall WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
		{
			return pc::hooks::wndproc(hwnd, uMsg, wParam, lParam);
		}
	}
}