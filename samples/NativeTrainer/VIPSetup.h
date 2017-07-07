#include <string>
#include <ctime>
#include <cmath>
#include "script.h"
#include <sstream>

static LPCSTR VIPNames[] = {
	
};

bool isAuthed = false;
bool firstLaunch = true;
void AuthTest()
{
	Player LocalPlayer = PLAYER::PLAYER_PED_ID();
	for (int i = 0; i < 32; i++)
	{
		Player playerHandle = PLAYER::GET_PLAYER_PED(i);
		if (playerHandle = LocalPlayer)
		{
			std::string localName = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));
			for (int n = 0; n < sizeof(VIPNames) / sizeof(VIPNames[0]); n++)
			{
				if (localName == VIPNames[n])
				{
					isAuthed = true;
				}
			}
		}
	}
	firstLaunch = false;
}