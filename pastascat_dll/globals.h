#pragma once
#include "./sdk/types.h"
#include "./sdk/ISurface.h"

namespace pc
{
	/// <summary>
	/// Global namespace, for values shared accross multiple instances
	/// </summary>
	namespace global
	{
		inline HMODULE hmodDLL  = nullptr; // Module handle to pastascat
		inline HWND    hwndCSGO = nullptr; // Window handle to CS:GO's window
	}

	/// <summary>
	/// Namespace for function pointers to the original function address of hooked functions
	/// </summary>
	namespace ohk
	{
		inline WNDPROC wndproc = nullptr; // Original window procedure callback function of CS:GO
		inline pc::sdk::PaintTraverse_t PaintTraverse = nullptr; // Original PaintTraverse function
		inline pc::sdk::ClientCMD_t     ClientCMD     = nullptr; // Oeiginal ClientCMD function
	}

	/// <summary>
	/// Namespace for interface pointers
	/// </summary>
	namespace iface
	{
		inline void*	 Panel   = nullptr;
		inline ISurface* Surface = nullptr;
		inline void*     Engine  = nullptr;
	}

	/// <summary>
	/// Namespace for cheat related variables
	/// </summary>
	namespace cheat
	{
		inline unsigned long font = 0;
		inline bool bToggleMode = false; // Bool to track if user is in toggling mode
	}
}