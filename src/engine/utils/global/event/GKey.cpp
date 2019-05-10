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

std::string GKey::getBindAsText(KeyBind p_keyBind) {
	std::string text = "";
	if (p_keyBind.key != -1) {
		if (p_keyBind.mods > 0) {
			if (p_keyBind.mods & GLFW_MOD_SUPER) { text += "Super+"; }
			if (p_keyBind.mods & GLFW_MOD_CONTROL) { text += "Ctrl+"; }
			if (p_keyBind.mods & GLFW_MOD_ALT) { text += "Alt+"; }
			if (p_keyBind.mods & GLFW_MOD_SHIFT) { text += "Shift+"; }
		}
		switch (p_keyBind.key) {
		case GLFW_KEY_F1:			text += "F1";		break;
		case GLFW_KEY_F2:			text += "F2";		break;
		case GLFW_KEY_F3:			text += "F3";		break;
		case GLFW_KEY_F4:			text += "F4";		break;
		case GLFW_KEY_F5:			text += "F5";		break;
		case GLFW_KEY_F6:			text += "F6";		break;
		case GLFW_KEY_F7:			text += "F7";		break;
		case GLFW_KEY_F8:			text += "F8";		break;
		case GLFW_KEY_F9:			text += "F9";		break;
		case GLFW_KEY_F10:			text += "F10";		break;
		case GLFW_KEY_F11:			text += "F11";		break;
		case GLFW_KEY_F12:			text += "F12";		break;
		case GLFW_KEY_SPACE:		text += "Space";	break;
		case GLFW_KEY_DELETE:		text += "Del";		break;
		case GLFW_KEY_BACKSPACE:	text += "Bksp";		break;
		default: text += (char)p_keyBind.key; break;
		}
	}
	return text;
}

std::vector<GKey::KeyEvent> GKey::getKeyEvents() {
	return m_keyEvents;
}

void GKey::reset() {
	for(KeyEvent ke : m_keyEvents)
		m_keyCommands[ke.keyCode].state = m_keyCommands[ke.keyCode].state & KeyState::KEY_DOWN;
	m_keyEvents.clear();
}
