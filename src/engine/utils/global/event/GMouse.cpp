#include "engine\utils\global\event\GMouse.h"
#include <iostream>

Vector2<Sint32> GMouse::m_mousePos, GMouse::m_dMousePos, GMouse::m_bMousePos;
bool GMouse::m_mouseMoved = false;
GMouse::MouseCommand GMouse::m_mouseCommands[32];
std::vector<GMouse::MouseEvent> GMouse::m_mouseEvents;
bool GMouse::m_mouseActive = false;
Sint8 GMouse::m_mouseScroll = 0;
Vector2<Sint32> GMouse::m_doubleclickPos;

void GMouse::mousePressCallback(GLFWwindow* p_window, Sint32 p_button, Sint32 p_state, Sint32 p_mods) {
	m_mouseEvents.push_back({p_button, p_state, p_mods});
	switch(p_state) {
	case GLFW_PRESS:
		m_mouseCommands[p_button].state = MouseState::MOUSE_PRESS | MouseState::MOUSE_DOWN;
		m_mouseCommands[p_button].mods = p_mods;
		if(m_mouseCommands[p_button].delay > 0
			&& (m_doubleclickPos - m_mousePos).getLength() < 4) {
			m_mouseCommands[p_button].state = m_mouseCommands[p_button].state | MouseState::MOUSE_DOUBLECLICK;
			m_mouseCommands[p_button].delay = 0;
		}
		else {
			m_mouseCommands[p_button].delay = 0.25f;
			m_doubleclickPos = m_mousePos;
		}
		break;
	case GLFW_RELEASE:
		m_mouseCommands[p_button].state = MouseState::MOUSE_RELEASE;
		break;
	}
}
void GMouse::mouseMovedCallback(GLFWwindow* p_window, GLdouble p_x, GLdouble p_y) {
	Sint32 x = Sint32(p_x), y = Sint32(p_y);
	m_mousePos = Vector2<Sint32>(x, y);
}
void GMouse::mouseScrollCallback(GLFWwindow* p_window, GLdouble p_x, GLdouble p_y) {
	m_mouseScroll += Sint8(p_y);
}

Vector2<Sint32> GMouse::getMousePos() {
	return m_mousePos;
}
Vector2<Sint32> GMouse::getDeltaMousePos() {
	return m_dMousePos;
}
bool GMouse::mouseMoved() {
	return (m_dMousePos.x != 0 || m_dMousePos.y != 0);
}
Sint8 GMouse::getMouseScroll() {
	return m_mouseScroll;
}

void GMouse::setMouseActive(bool p_active) {
	m_mouseActive = p_active;
}

bool GMouse::mousePressed(Sint32 p_mouse, Sint32 p_mods) {
	return (m_mouseCommands[p_mouse].state & MouseState::MOUSE_PRESS) && (p_mods == -1 || m_mouseCommands[p_mouse].mods == p_mods);
}
bool GMouse::mouseDoubleClicked(Sint32 p_mouse, Sint32 p_mods) {
	return (m_mouseCommands[p_mouse].state & MouseState::MOUSE_DOUBLECLICK) && (p_mods == -1 || m_mouseCommands[p_mouse].mods == p_mods);
}
bool GMouse::mouseDown(Sint32 p_mouse, Sint32 p_mods) {
	return (m_mouseCommands[p_mouse].state & MouseState::MOUSE_DOWN) && (p_mods == -1 || m_mouseCommands[p_mouse].mods == p_mods);
}
bool GMouse::mouseReleased(Sint32 p_mouse) {
	return (m_mouseCommands[p_mouse].state & MouseState::MOUSE_RELEASE);
}

void GMouse::reset() {
	m_mouseScroll = 0;
	for(MouseCommand& mc : m_mouseCommands)
		mc.state = mc.state & MouseState::MOUSE_DOWN;
}

void GMouse::update(GLfloat p_deltaTime) {
	m_dMousePos = m_mousePos - m_bMousePos;
	m_bMousePos = m_mousePos;
	for(MouseCommand& mc : m_mouseCommands)
		if(mc.delay >= 0) mc.delay -= p_deltaTime;
}
