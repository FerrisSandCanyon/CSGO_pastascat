#pragma once

#include <Windows.h>
#include "../globals.h"

namespace pc
{
	/// <summary>
	/// Namespace for hooked functions
	/// </summary>
	namespace hooked
	{
		LRESULT __stdcall WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	}
}