#include "engine\gfx\gui\global\GGui.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\gfx\gui\component\Component.h"

GLFWwindow* GGui::m_mainWindow = 0;
GGui::CursorType GGui::m_cursorType = GGui::CursorType::NONE;
std::map<GGui::CursorType, GLFWcursor*> GGui::m_cursors;
std::string GGui::m_tooltip = "";
Vector2<Sint32> GGui::m_tooltipPos = {};
bool GGui::m_tooltipFading = false;
GLfloat GGui::m_tooltipLife = 0;
GLfloat GGui::m_tooltipMaxLife = 0.1f;

void GGui::init(GLFWwindow* p_mainWindow) {
	m_mainWindow = p_mainWindow;

	glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

	Texture* cursor;
	m_cursors.emplace(CursorType::NONE,				glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
	m_cursors.emplace(CursorType::IBEAM,			glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
	m_cursors.emplace(CursorType::LINK,				glfwCreateStandardCursor(GLFW_HAND_CURSOR));
	m_cursors.emplace(CursorType::EWRESIZE,			glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
	m_cursors.emplace(CursorType::NSRESIZE,			glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
	cursor = MTexture::getTexture("cursor\\aero_nesw.png", true);
	m_cursors.emplace(CursorType::NESWRESIZE,		glfwCreateCursor(cursor->getGlfwImage(), cursor->getSize().x / 2, cursor->getSize().y / 2));
	cursor = MTexture::getTexture("cursor\\aero_nwse.png", true);
	m_cursors.emplace(CursorType::NWSERESIZE,		glfwCreateCursor(cursor->getGlfwImage(), cursor->getSize().x / 2, cursor->getSize().y / 2));

	/*m_cursors.emplace(CursorType::IBEAM,		LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::IBEAM)));
	m_cursors.emplace(CursorType::LINK,			LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::LINK)));
	m_cursors.emplace(CursorType::EWRESIZE,		LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::EWRESIZE)));
	m_cursors.emplace(CursorType::NSRESIZE,		LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::NSRESIZE)));
	m_cursors.emplace(CursorType::NESWRESIZE,	LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::NESWRESIZE)));
	m_cursors.emplace(CursorType::NWSERESIZE,	LoadCursorA(hInstance, MAKEINTRESOURCE(CursorType::NWSERESIZE)));*/

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
	if (m_cursorType != CursorType::NONE) {
		//SetCursor(m_cursors.at(m_cursorType));
		m_cursorType = CursorType::NONE;
	}
	if (m_tooltipFading) {
		m_tooltipLife += GScreen::getDeltaUpdate();
		if (m_tooltipLife >= m_tooltipMaxLife) {
			m_tooltipLife = m_tooltipMaxLife;
		}
	}
	else if (m_tooltipLife > 0) {
		m_tooltipLife -= GScreen::getDeltaUpdate();
		if (m_tooltipLife <= 0) {
			m_tooltipLife = 0;
			m_tooltip = "";
		}
	}
	m_tooltipFading = false;
}
