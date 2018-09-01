#include "engine\gfx\gui\global\GGui.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\gfx\gui\component\Component.h"

GLFWwindow* GGui::m_mainWindow = 0;
GGui::CursorType GGui::m_cursorType = GGui::CursorType::ARROW;
std::map<GGui::CursorType, GLFWcursor*> GGui::m_cursors;
std::string GGui::m_tooltip = "";
Vector2<Sint32> GGui::m_tooltipPos = {};
GLfloat GGui::m_tooltipLife = 0;

void GGui::init(GLFWwindow* p_mainWindow) {
	m_mainWindow = p_mainWindow;
	m_cursors.emplace(CursorType::ARROW,		glfwCreateStandardCursor(static_cast<int>(CursorType::ARROW)));
	m_cursors.emplace(CursorType::IBEAM,		glfwCreateStandardCursor(static_cast<int>(CursorType::IBEAM)));
	m_cursors.emplace(CursorType::CROSSHAIR,	glfwCreateStandardCursor(static_cast<int>(CursorType::CROSSHAIR)));
	m_cursors.emplace(CursorType::HAND,			glfwCreateStandardCursor(static_cast<int>(CursorType::HAND)));
	m_cursors.emplace(CursorType::HRESIZE,		glfwCreateStandardCursor(static_cast<int>(CursorType::HRESIZE)));
	m_cursors.emplace(CursorType::VRESIZE,		glfwCreateStandardCursor(static_cast<int>(CursorType::VRESIZE)));

	Component::init();
}
void GGui::terminate() {
	m_cursors.clear();
	Component::terminate();
}

void GGui::setTooltip(std::string p_tooltip, Vector2<Sint32> p_pos) {
	if(m_tooltip != p_tooltip) {
		m_tooltip = p_tooltip;
		m_tooltipPos = p_pos;
	}
	m_tooltipLife = (GLfloat)glfwGetTime() + 0.5f;
}
std::string GGui::getTooltip() {
	if(m_tooltipLife - glfwGetTime() < 0)
		m_tooltip = "";
	return m_tooltip;
}
Vector2<Sint32> GGui::getTooltipPos() {
	return m_tooltipPos;
}
GGui::CursorType GGui::getCursorType() {
	return m_cursorType;
}
void GGui::setCursorType(CursorType p_cursorType) {
	m_cursorType = p_cursorType;
}

void GGui::update() {
	glfwSetCursor(m_mainWindow, m_cursors.at(m_cursorType));
	m_cursorType = CursorType::ARROW;
}
