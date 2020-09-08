#pragma once
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
	namespace hooks
	{
		inline WNDPROC wndproc = nullptr; // Original window procedure callback function of CS:GO
	}

	/// <summary>
	/// Namespace for interface pointers
	/// </summary>
	namespace iface
	{
		
	}
}