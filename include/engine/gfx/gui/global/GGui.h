#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\LOpenGL.h"
#include <string>
#include <map>

class GGui {
public:
	enum class CursorType {
		NONE =			GLFW_ARROW_CURSOR,
		IBEAM =			GLFW_IBEAM_CURSOR,
		LINK =			GLFW_HAND_CURSOR,
		EWRESIZE =		GLFW_HRESIZE_CURSOR,
		NSRESIZE =		GLFW_VRESIZE_CURSOR,
		NESWRESIZE =	6,
		NWSERESIZE =	7
	};
private:
	static GLFWwindow*							m_mainWindow;
	static CursorType							m_cursorType;
	static std::map<CursorType, GLFWcursor*>	m_cursors;
	static std::string							m_tooltip;
	static Vector2<Sint32>						m_tooltipPos;
	static bool									m_tooltipFading;
	static GLfloat								m_tooltipLife; // Time at which tooltip disappears if not refreshed
	static GLfloat								m_tooltipMaxLife;
public:
	static void init(GLFWwindow* p_mainWindow);
	static void terminate();

	static void setTooltip(std::string p_tooltip, Vector2<Sint32> p_pos);
	static std::string getTooltip();
	static Vector2<Sint32> getTooltipPos();
	static GLfloat getTooltipFade();
	static CursorType getCursorType();
	static void setCursorType(CursorType p_cursorType);

	static void update();
};
