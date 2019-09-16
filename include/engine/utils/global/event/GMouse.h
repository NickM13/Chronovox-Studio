#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\LOpenGL.h"
#include <vector>

// Global class for Mouse events
class GMouse {
public:
	struct MouseEvent {
		Sint32 button, state, mods;
	};
private:
	struct MouseCommand {
		Sint32 state, mods;
		GLfloat delay;
	};
	enum MouseState {
		MOUSE_PRESS = 1,		// Mouse just pressed
		MOUSE_DOWN = 2,			// Mouse held down
		MOUSE_RELEASE = 4,		// Mouse just released
		MOUSE_DOUBLECLICK = 8
	};
	static Vector2<Sint32> m_mousePos, m_dMousePos, m_bMousePos;
	static MouseCommand m_mouseCommands[32];
	static Sint8 m_mouseScroll;
	static bool m_mouseMoved;
	static bool m_mouseActive;
	static Vector2<Sint32> m_doubleclickPos;
	static std::vector<MouseEvent> m_mouseEvents;
public:
	static void mousePressCallback(GLFWwindow* p_window, Sint32 p_button, Sint32 p_state, Sint32 p_mods);
	static void mouseMovedCallback(GLFWwindow* p_window, GLdouble x, GLdouble y);
	static void mouseScrollCallback(GLFWwindow* p_window, GLdouble x, GLdouble y);

	static Vector2<Sint32> getMousePos();
	static Vector2<Sint32> getDeltaMousePos();
	static bool mouseMoved();
	static Sint8 getMouseScroll();

	static void setMouseActive(bool p_active);
	static bool isMouseActive() { return m_mouseActive; }
	static bool mousePressed(Sint32 p_mouse, Sint32 p_mods = -1);
	static bool mouseDoubleClicked(Sint32 p_mouse, Sint32 p_mods = -1);
	static bool mouseDown(Sint32 p_mouse, Sint32 p_mods = -1);
	static bool mouseReleased(Sint32 p_mouse);

	static void reset();
	static void update(GLfloat p_deltaTime);
};
