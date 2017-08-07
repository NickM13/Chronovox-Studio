#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include <vector>

class KeyStates
{
public:
	enum Event
	{
		KEY_PRESS = 1,
		KEY_HOLD = 2,
		KEY_RELEASE = 4
	};

	static Sint8 m_keyStates[1024];
	static Sint8 m_specStates[256];
	struct keyPress
	{
		Sint32 m_keyCode;
		Sint32 m_scanCode;
		Sint32 m_action;
		Sint32 m_mods;
	};
	static std::vector<keyPress> m_keyEvents;
};

