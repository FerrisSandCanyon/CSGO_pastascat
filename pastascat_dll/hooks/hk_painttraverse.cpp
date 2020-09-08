#include "./hk.h"
#include "../../pastascat_sdk/hash.h"

namespace pc
{
	namespace hooked
	{
		void __fastcall PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce)
		{
			static unsigned int nMatSystemTopPanel = 0;

			if (nMatSystemTopPanel == 0 && pc::utils::cx_fnv(reinterpret_cast<const char* (__thiscall*)(void*, unsigned int)>(reinterpret_cast<void***>(pc::iface::Panel)[0][36])(pc::iface::Panel, panel), 17) == pc::utils::cx_fnv("MatSystemTopPanel"))
				nMatSystemTopPanel = panel;

			if (panel == nMatSystemTopPanel)
			{
				pc::iface::Surface->DrawTextW(L"pastascat", 12, 30, pc::cheat::font, 255, 192, 203);
			}

			pc::ohk::PaintTraverse(ecx, panel, forceRepaint, allowForce);
		}
	}
}