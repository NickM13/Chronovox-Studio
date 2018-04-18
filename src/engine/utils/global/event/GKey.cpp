#include "engine\utils\global\event\GKey.h"
#include <iostream>

GKey::KeyCommand GKey::m_keyCommands[1024];
std::vector<GKey::KeyEvent> GKey::m_keyEvents;

void GKey::keyCallback(GLFWwindow* p_window, Sint32 p_keyCode, Sint32 p_scanCode, Sint32 p_action, Sint32 p_mods) {
	m_keyEvents.push_back({p_keyCode, p_scanCode, p_action, p_mods});
	switch(p_action) {
	case GLFW_PRESS:
		m_keyCommands[p_keyCode].mods = p_mods;
	case GLFW_REPEAT:
		m_keyCommands[p_keyCode].state = KeyState::KEY_PRESS | KeyState::KEY_DOWN;
		break;
	case GLFW_RELEASE:
		m_keyCommands[p_keyCode].state = KeyState::KEY_RELEASE;
		break;
	}
}
bool GKey::keyPressed(Sint32 p_key, Sint32 p_mods) {
	return (m_keyCommands[p_key].state & KeyState::KEY_PRESS) && (p_mods == -1 || m_keyCommands[p_key].mods == p_mods);
}
bool GKey::keyDown(Sint32 p_key, Sint32 p_mods) {
	return (m_keyCommands[p_key].state & KeyState::KEY_DOWN) && (p_mods == -1 || m_keyCommands[p_key].mods == p_mods);
}
bool GKey::keyRelease(Sint32 p_key) {
	return (m_keyCommands[p_key].state & KeyState::KEY_RELEASE);
}
bool GKey::modDown(Sint32 p_mod) {
	if(p_mod == GLFW_MOD_ALT) return (keyDown(GLFW_KEY_LEFT_ALT) || keyDown(GLFW_KEY_RIGHT_ALT));
	if(p_mod == GLFW_MOD_CONTROL) return (keyDown(GLFW_KEY_LEFT_CONTROL) || keyDown(GLFW_KEY_RIGHT_CONTROL));
	if(p_mod == GLFW_MOD_SHIFT) return (keyDown(GLFW_KEY_LEFT_SHIFT) || keyDown(GLFW_KEY_RIGHT_SHIFT));
	if(p_mod == GLFW_MOD_SUPER) return (keyDown(GLFW_KEY_LEFT_SUPER) || keyDown(GLFW_KEY_RIGHT_SUPER));
	return false;
}

std::vector<GKey::KeyEvent> GKey::getKeyEvents() {
	return m_keyEvents;
}

void GKey::reset() {
	for(KeyEvent ke : m_keyEvents)
		m_keyCommands[ke.keyCode].state = m_keyCommands[ke.keyCode].state & KeyState::KEY_DOWN;
	m_keyEvents.clear();
}
