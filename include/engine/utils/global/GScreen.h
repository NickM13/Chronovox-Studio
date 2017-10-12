#pragma once

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\OpenGL.h"
#include <vector>
#include <string>
#include <iostream>

class GScreen
{
public:
	static std::string m_tooltip;
	static float m_fps;
	static float m_fov;
	static float m_deltaTime;
	static Sint8 m_exitting;
	static bool m_developer;

	static GLFWwindow *m_window;
	static Vector2<Sint32> m_monitorSize;
	static Vector2<Sint32> m_windowPos;
	static bool m_maximized;
	static Vector2<Sint32> m_screenSize;
	static Vector2<Sint32> m_smallScreen;
	static bool m_draggingWindow;
	static Vector2<Sint32> m_dragStart;
	static Vector2<Sint32> m_dragDistance;
	static Vector2<Sint32> m_initWindowSize;
	static Vector2<Sint32> m_resizeMousePos;
	static bool m_resizing;
	static bool m_finishedResize;

	static enum WindowCommand
	{
		NONE = 0,
		CLOSE,
		RESIZE,
		MINIMIZE
	} m_windowCommand;

	static void initWindow(GLFWwindow *p_window);

	static bool isMaximized() { return m_maximized; }
	static void startWindowDrag();
	static bool isDraggingWindow();
	static void endWindowDrag();

	static void startResizing();
	static bool finishedResize() { return m_finishedResize; }
	static void stopResizing();

	static void updateWindow();
};
