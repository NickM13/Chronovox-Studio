#include "engine\utils\global\event\GMouse.h"

Vector2<Sint32> GMouse::m_mousePos = {};
Vector2<Sint32> GMouse::m_guiMousePos = {};
bool GMouse::m_mouseMoved = false;
Sint8 GMouse::m_mouseStates[32];
float GMouse::m_mouseDelay[32];
Sint8 GMouse::m_mouseScroll = 0;
