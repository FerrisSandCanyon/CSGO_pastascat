#pragma once

#include <Windows.h>
#include "../globals.h"
#include "../sdk/types.h"

namespace pc
{
	/// <summary>
	/// Namespace for hooked functions
	/// </summary>
	namespace hooked
	{
		LRESULT __stdcall WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		void __fastcall PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce);
		void __fastcall ClientCMD(void* ecx, void* edx, const char* szCmdString);
	}
}