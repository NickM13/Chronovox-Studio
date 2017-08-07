#include "engine\utils\Globals.h"

std::string Globals::m_tooltip = "";
float Globals::m_fps = 0;
float Globals::m_fov = 0;
float Globals::m_deltaTime = 0;
Vector2<Sint32> Globals::m_screenSize = {};
bool Globals::m_developer = false;
Sint8 Globals::m_exitting = 0;
Globals::WindowCommand Globals::m_windowCommand = NONE;
float Globals::m_timeWarp = 0;
Globals::GameState Globals::m_gameState = Globals::GameState::MENU_MAIN;
