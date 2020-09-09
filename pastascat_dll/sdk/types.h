#pragma once

namespace pc
{
	namespace sdk
	{
		typedef void(__thiscall* PaintTraverse_t)(void*, unsigned int, bool, bool);
		typedef void(__thiscall* ClientCMD_t)(void*, const char*);
	}
}