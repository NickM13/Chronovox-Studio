#include "engine\utils\event\KeyStates.h"

Sint8 KeyStates::m_keyStates[1024];
Sint8 KeyStates::m_specStates[256];
std::vector<KeyStates::keyPress> KeyStates::m_keyEvents;
