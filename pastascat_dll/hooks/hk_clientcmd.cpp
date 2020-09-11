#include "./hk.h"

namespace pc
{
	namespace hooked
	{
		void __fastcall ClientCMD(void* ecx, void* edx, const char* szCmdString)
		{
			unsigned int offset = 0;
			if (
				szCmdString[0] != 's' || szCmdString[1] != 'a' || szCmdString[2] != 'y'                     // Say command check
			||  szCmdString[offset = (szCmdString[3] == '_' ? sizeof("say_team ") : sizeof("say "))] != '!' // Command prefix check
			) {
				return pc::ohk::ClientCMD(ecx, szCmdString);
			}

			bool bAllChat = offset == sizeof("say ");
			szCmdString += offset + 1; // push the szCmdString pointer to point to the actual command (+1 for the ! prefix)

			// Do command stuff here
			

		}
	}
}