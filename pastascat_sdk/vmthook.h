#pragma once

namespace pc
{
	namespace utils
	{		
		/// <summary>
		/// Performs a Virtual Method Table Hook by replacing the function pointer
		/// </summary>
		/// <param name="instance">Instance to hook</param>
		/// <param name="idx">Virtual table index to hook</param>
		/// <param name="fnNew">Pointer to the new hooked function</param>
		/// <param name="pStoreOld">[out] Returns the previous function pointer before the hook</param>
		/// <returns></returns>
		bool VMTHook(void*** instance, int idx, void* fnNew, void*& pStoreOld);
	}
}