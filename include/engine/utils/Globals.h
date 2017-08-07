#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\Singleton.h"
#include <vector>
#include <string>

class Globals
{
public:
	static std::string m_tooltip;
	static float m_fps;
	static float m_fov;
	static float m_deltaTime;
	static Vector2<Sint32> m_screenSize;
	static bool m_developer;
	static Sint8 m_exitting;
	
	static enum WindowCommand
	{
		NONE = 0,
		CLOSE,
		MINIMIZE
	} m_windowCommand;

	static float m_timeWarp;


	static enum GameState
	{
		MENU_START,
		MENU_MAIN,
		GAME_PAUSE,
		GAME_PLAY
	} m_gameState;
};
