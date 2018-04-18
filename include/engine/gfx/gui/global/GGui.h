#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\LOpenGL.h"
#include <string>

class GGui {
public:
	enum class MouseType {
		ARROW = GLFW_ARROW_CURSOR,
		IBEAM = GLFW_IBEAM_CURSOR,
		CROSSHAIR = GLFW_CROSSHAIR_CURSOR,
		HAND = GLFW_HAND_CURSOR,
		HRESIZE = GLFW_HRESIZE_CURSOR,
		VRESIZE = GLFW_VRESIZE_CURSOR
	};
private:
	static GLFWwindow*		m_mainWindow;
	static GLFWcursor*		m_cursor;
	static std::string		m_tooltip;
	static Vector2<Sint32>	m_tooltipPos;
	static GLfloat			m_tooltipLife; // Time at which tooltip disappears if not refreshed
	static MouseType		m_mouseType;
public:
	static void init(GLFWwindow* p_mainWindow);
	static void setTooltip(std::string p_tooltip, Vector2<Sint32> p_pos);
	static std::string getTooltip();
	static Vector2<Sint32> getTooltipPos();
	static MouseType getMouseType();
	static void setMouseType(MouseType p_mouseType);

	static void update();
};
