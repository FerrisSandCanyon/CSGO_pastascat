#include "./vmthook.h"

namespace pc
{
	namespace utils
	{
		bool VMTHook(void*** instance, int idx, void* fnNew, void*& old)
		{
			old = instance[0][idx];
			instance[0][idx] = fnNew;

			return true;
		}
	}
}