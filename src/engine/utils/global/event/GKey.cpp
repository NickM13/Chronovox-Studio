#include "engine\utils\global\event\GKey.h"

Sint8 GKey::m_keyStates[1024];
Sint8 GKey::m_specStates[256];
std::vector<GKey::keyPress> GKey::m_keyEvents;
