#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"

// Global class for Mouse events
class GMouse
{
public:
	enum Event {
		MOUSE_DOWN = 1,			// Mouse held down
		MOUSE_PRESS = 2,		// Mouse just pressed
		MOUSE_RELEASE = 4,		// Mouse just released
		MOUSE_DOUBLECLICK = 8
	};

	static Vector2<Sint32> m_mousePos;
	static Vector2<Sint32> m_guiMousePos;
	static Sint8 m_mouseStates[32];
	static float m_mouseDelay[32];
	static Sint8 m_mouseScroll;
	static bool m_mouseMoved;

	static void addMouseEvent(Sint8 p_mouse, Event p_event) {
		if(!(m_mouseStates[p_mouse] & p_event))
			 m_mouseStates[p_mouse] += p_event;
	}
};
