#include "engine\gfx\gui\global\GGui.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\gfx\gui\component\Component.h"

GLFWwindow* GGui::m_mainWindow = 0;
GGui::CursorType GGui::m_cursorType = GGui::CursorType::ARROW;
std::map<GGui::CursorType, GLFWcursor*> GGui::m_cursors;
std::string GGui::m_tooltip = "";
Vector2<Sint32> GGui::m_tooltipPos = {};
bool GGui::m_tooltipFading = false;
GLfloat GGui::m_tooltipLife = 0;
GLfloat GGui::m_tooltipMaxLife = 0.1f;

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
	m_tooltipFading = true;
}
std::string GGui::getTooltip() {
	return m_tooltip;
}
Vector2<Sint32> GGui::getTooltipPos() {
	return m_tooltipPos;
}
GLfloat GGui::getTooltipFade() {
	return m_tooltipLife / m_tooltipMaxLife;
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
	if (m_tooltipFading) {
		m_tooltipLife += GScreen::m_deltaTime;
		if (m_tooltipLife >= m_tooltipMaxLife) {
			m_tooltipLife = m_tooltipMaxLife;
		}
	}
	else if (m_tooltipLife > 0) {
		m_tooltipLife -= GScreen::m_deltaTime;
		if (m_tooltipLife <= 0) {
			m_tooltipLife = 0;
			m_tooltip = "";
		}
	}
	m_tooltipFading = false;
}
