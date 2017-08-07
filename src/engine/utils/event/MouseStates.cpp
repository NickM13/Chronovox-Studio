#include "engine\utils\event\MouseStates.h"

Vector2<Sint32> MouseStates::m_mousePos = {};
Vector2<Sint32> MouseStates::m_guiMousePos = {};
bool MouseStates::m_mouseMoved = false;
Sint8 MouseStates::m_mouseStates[64];
float MouseStates::m_mouseDelay[64];
Sint8 MouseStates::m_mouseScroll = 0;
