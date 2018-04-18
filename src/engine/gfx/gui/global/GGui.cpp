#include "engine\gfx\gui\global\GGui.h"
#include "engine\utils\global\event\GMouse.h"

GLFWwindow* GGui::m_mainWindow = 0;
GLFWcursor* GGui::m_cursor = 0;
std::string GGui::m_tooltip = "";
Vector2<Sint32> GGui::m_tooltipPos = {};
GLfloat GGui::m_tooltipLife = 0;
GGui::MouseType GGui::m_mouseType = GGui::MouseType::ARROW;

void GGui::init(GLFWwindow* p_mainWindow) {
	m_mainWindow = p_mainWindow;
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
GGui::MouseType GGui::getMouseType() {
	return m_mouseType;
}
void GGui::setMouseType(MouseType p_mouseType) {
	m_mouseType = p_mouseType;
}

void GGui::update() {
	if(m_cursor) glfwDestroyCursor(m_cursor);
	m_cursor = glfwCreateStandardCursor(static_cast<int>(m_mouseType));
	glfwSetCursor(m_mainWindow, m_cursor);
	m_mouseType = MouseType::ARROW;
}
